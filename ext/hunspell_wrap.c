#include <ruby.h>
#include <hunspell/hunspell.h>

static VALUE rb_cHunspell;
static VALUE rb_cIconv;
static VALUE enc_utf8;
static VALUE enc_iso8859_1;

static void dict_destroy(Hunhandle *handle) {
    Hunspell_destroy(handle);
}

/*
 *  call-seq:
 *     Hunspell.new(affix_path, dic_path)
 *
 *  Instantiate Hunspell with paths pointing to affix and dictionary files.
 *
 *  Example:
 *    speller = Hunspell.new("/usr/share/myspell/en_US.aff", "/usr/share/myspell/en_US.dic")
 *
 */
static VALUE dict_init(VALUE self, VALUE affix_path, VALUE dic_path) {
    VALUE affpath = StringValue(affix_path), dpath = StringValue(dic_path);
    Hunhandle *handle = Hunspell_create(RSTRING_PTR(affpath), RSTRING_PTR(dpath));
    if (!handle) {
      rb_raise(rb_eRuntimeError, "Failed to initialize Hunspell handle.");
    }
    VALUE dict = Data_Wrap_Struct(rb_cHunspell, NULL, dict_destroy, handle);
    rb_iv_set(self, "dict", dict);
    return self;
}

static inline Hunhandle* get_handle(VALUE self) {
    Hunhandle *handle = NULL;
    Data_Get_Struct(rb_iv_get(self, "dict"), Hunhandle, handle);
    if (!handle) {
      rb_raise(rb_eRuntimeError, "Hunspell handle not found.");
    }
    return handle;
}

/*
 *  call-seq:
 *     hunspell.encoding -> string
 *
 *  Returns the encoding of the dictionary.
 */
static VALUE wrap_encoding(VALUE self) {
    Hunhandle *handle = get_handle(self);
    char *enc = Hunspell_get_dic_encoding(handle);
    return rb_str_new2(enc);
}

static VALUE recode_if_needed(VALUE self, VALUE str, int dir) {
    VALUE enc = wrap_encoding(self);
    VALUE from, to;

    if (dir == 0) {
        from = enc_utf8;
        to = enc;
    } else {
        from = enc;
        to = enc_utf8;
    }
    if ( rb_str_equal(enc, enc_iso8859_1) == Qfalse && rb_str_equal(enc, enc_utf8) == Qfalse) {
        rb_funcall(str, "force_encoding", 1, enc_utf8);
        return rb_funcall(rb_cIconv, rb_intern("conv"), 3, to, from, str);
    } else {
        return str;
    }
}

/*
 *  call-seq:
 *     hunspell.suggest(misspeledword) -> ary
 *
 *  Returns a list of suggestions.
 */
static VALUE wrap_suggest(VALUE self, VALUE word) {
    VALUE str = recode_if_needed(self, StringValue(word), 0);
    VALUE res;
    char** slst = NULL;
    int i, count = 0;
    Hunhandle *handle = get_handle(self);

    count = Hunspell_suggest(handle, &slst, RSTRING_PTR(str));

    res = rb_ary_new2(count);
    for (i=0; i<count; ++i) {
        rb_ary_push(res, recode_if_needed(self, rb_str_new2(slst[i]), 1));
        free(slst[i]);
    }

    if (slst) {
        free(slst);
    }

    return res;
}

/*
 *  call-seq:
 *     hunspell.valid?(word) -> bool
 *
 *  Checks if the word is in the dictionary.
 */
static VALUE wrap_check(VALUE self, VALUE word) {
    VALUE str = recode_if_needed(self, StringValue(word), 0);
    Hunhandle *handle = get_handle(self);
    int rc = Hunspell_spell(handle, RSTRING_PTR(str));
    return rc == 0 ? Qfalse : Qtrue;
}


void Init_hunspell(void) {
    rb_cHunspell = rb_define_class("Hunspell", rb_cObject);
    rb_define_method(rb_cHunspell, "initialize", dict_init, 2);
    //rb_define_method(rb_cHunspell, "check?", wrap_check, 1);
    rb_define_method(rb_cHunspell, "valid?", wrap_check, 1);
    rb_define_method(rb_cHunspell, "suggest", wrap_suggest, 1);
    rb_define_method(rb_cHunspell, "encoding", wrap_encoding, 0);
    rb_require("iconv");
    rb_cIconv = rb_const_get(rb_cObject, rb_intern("Iconv"));
    enc_iso8859_1 = rb_str_new2("ISO8859-1");
    enc_utf8 = rb_str_new2("UTF-8");
    rb_global_variable(&enc_iso8859_1);
    rb_global_variable(&enc_utf8);
}
