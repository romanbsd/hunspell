require 'mkmf'

unless have_header('hunspell/hunspell.h') and (have_library('hunspell-1.2') or have_library('hunspell-1.3'))
  raise 'Hunspell not found.'
end

create_makefile('hunspell')
