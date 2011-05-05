require 'spec_helper'

describe Hunspell do

  before :all do
    @speller = Hunspell.new('en_US.aff', 'en_US.dic')
  end

  it "should check if a word is valid" do
    @speller.should be_valid('dog')
    @speller.should_not be_valid('dxg')
  end

  it "survives GC" do
    10.times do
      @speller.suggest('dxg')
    end
    GC.start
    10.times do
      @speller.suggest('dxg')
    end
  end

  it "should suggest alternate spellings for words" do
    @speller.suggest('arbitrage').should == %w[
      arbitrage
      arbitrages
      arbitrager
      arbitraged
      arbitrate
    ]
  end
end
