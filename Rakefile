require 'rake'
require 'rake/rdoctask'

desc 'Default: create gemspec'
task :default => :gemspec

desc 'Generate documentation'
Rake::RDocTask.new(:rdoc) do |rdoc|
  rdoc.rdoc_dir = 'rdoc'
  rdoc.title    = 'Hunspell'
  rdoc.options << '--line-numbers' << '--inline-source'
  rdoc.rdoc_files.include('README')
  rdoc.rdoc_files.include('lib/**/*.rb')
end

begin
  require 'jeweler'
  Jeweler::Tasks.new do |gem|
    gem.name = "hunspell-i18n"
    gem.summary = %Q{Ruby bindings for libhunspell-1.2 with i18n support}
    gem.description = gem.summary
    gem.email = "romanbsd@yahoo.com"
    gem.homepage = "http://github.com/romanbsd/hunspell"
    gem.authors = ["Roman Shterenzon"]
    gem.extensions = ['ext/extconf.rb']
  end
  Jeweler::GemcutterTasks.new
rescue LoadError
end
