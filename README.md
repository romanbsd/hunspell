# Ruby bindings for Hunspell 1.2.x

This version expects to receive the word in UTF-8 encoding, and it will
then use iconv in order to convert to and from dictionary encoding.

## Usage

    prefix = '/usr/share/myspell/'  # Fedora
    speller = Hunspell.new(prefix + 'en_US.aff', prefix + 'en_US.dic')
    speller.valid?('dog')           # -> true
    speller.valid?('dpg')           # -> false
    speller.suggest('dpg')          # -> ["dog", "dg", "pg", "deg", "dig", ...]


Copyright 2011 Roman Shterenzon
