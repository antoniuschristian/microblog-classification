import sys
import re
import string
from nltk.corpus import stopwords

'''
    Parse the tweet text to remove special tweets char
'''
def parsedTweets(tweet):
    #Convert www.* or https?://* to URL
    parsedTweet = re.sub('((www\.[\s]+)|(https?://[^\s]+))','URL',tweet)
    #remove @username
    parsedTweet = re.sub('@[^\s]+','',parsedTweet)
    #remove punctuation
    parsedTweet = parsedTweet.translate(string.maketrans("",""), string.punctuation)
    #Remove additional white spaces
    parsedTweet = " ".join(parsedTweet.split())
    #Replace #word with word
    parsedTweet = re.sub(r'#([^\s]+)', r'\1', parsedTweet)
    #trim
    parsedTweet = parsedTweet.strip('\'"')
    return parsedTweet

'''
    Generate dictionary for use in informal language normalization
'''
TwitterDictionaryWords = {}
def getTwitterDictionaryWordList(TwitterDictionaryFileName):
    try:
        fileReader = open(TwitterDictionaryFileName, 'r')
        for line in fileReader:
            line = " ".join(line.split())
            line = line.split()
            TwitterDictionaryWords[line[0]] = line[1]
        fileReader.close()
    except IOError:
        sys.stderr.write('err reading:' + TwitterDictionaryFileName)
    return TwitterDictionaryWords

'''
    Read and Store the text file from Suffendy
    TweetsCorpus[i]["text"] will get the text for the no. i tweet
'''
def parsedTextFile(in_file):
    TweetsCorpus = []
    new_dict = {}
    try:
        fileReader = open(in_file, 'r')
        for line in fileReader:
            if line.find("#") == 0:
                TweetsCorpus.append(new_dict)
                new_dict = {}
            else:
                key = line[:(line.index("--")-1)]
                value = line[(line.index("--")+3):].rstrip().lower()
                new_dict[key]=value
        fileReader.close()
    except IOError:
        sys.stderr.write('err reading:' + in_file)
    return TweetsCorpus

'''
    Not sure if its needed
'''
def LanguageIdenfitication(Corpus):
    return Corpus

'''
    Replace informal words
'''
def LanguageNormalization(Corpus):
    for Tweet in Corpus:
        parsedTweet = parsedTweets(Tweet["text"])
        newTweetBase = []
        tweetBase = parsedTweet.split(' ')
        for word in tweetBase:
            if TwitterDictionaryWords.has_key(word):
                word = TwitterDictionaryWords[word]
            newTweetBase.append(word)
        Tweet["text"] = newTweetBase
    return Corpus

'''
    Stopwords Removal
'''
def IrrelvantTextFilter(Corpus):
    stop = stopwords.words("english")
    for Tweet in Corpus:
        parsedTweet = Tweet["text"]
        newTweetBase = []
        newTweetBase = [i for i in parsedTweet if i not in stop]
        Tweet["text"] = newTweetBase
    return Corpus

'''
    Main Pipeline for text processing
'''
def preprocessPipeline(in_file):
    TwitterDictionaryWords = getTwitterDictionaryWordList('..\Dictionary\emnlp_dict.txt')
    TweetsCorpus = parsedTextFile(in_file);
    LanguageNormalizedTweetsCorpus = LanguageNormalization(TweetsCorpus)
    IrrelvantTextFilterTweetsCorpus = IrrelvantTextFilter(LanguageNormalizedTweetsCorpus)
    return IrrelvantTextFilterTweetsCorpus

def main():
    ProcessedTweetsCorpus = preprocessPipeline('..\Data\Processed\parsedTweets.txt')
    print ProcessedTweetsCorpus

if __name__ == '__main__':
    main()