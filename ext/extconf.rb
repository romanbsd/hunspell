require 'mkmf'

unless have_header('hunspell/hunspell.h') and have_library('hunspell-1.2')
  raise 'Hunspell not found.'
end

create_makefile('hunspell')
