import json
import types
import glob
import unicodedata
import codecs


def f(output,p,a,b):
	if str(type(b)) == '<type \'dict\'>':
		p = p + '-'
		for c in b:
			f(output,p+c,c,b[c])
	elif str(type(b)) == '<type \'list\'>':
		for c in range(len(b)):
			f(output,p,c,b[c])
	else:
		if str(type(b)) == '<type \'unicode\'>':
			b = unicodedata.normalize( 'NFKD', b ).encode( 'ascii', 'ignore' )
		b = str(b)
		b = b.replace('\r','\\r')
		b = b.replace('\n','\\n')
		L = [p, '--', b]
		output.write(' '.join(L) + '\n')


fpath = './TRAIN/*.txt'
files = glob.glob(fpath)
for file in files:
	output = open( file + '.p', 'w' )
	for line in codecs.open( file, encoding='utf-8' ):
		data = json.loads(line)
		for x in data:
			if str(type(data[x])) == '<type \'unicode\'>':
				data[x] = unicodedata.normalize( 'NFKD', data[x] ).encode( 'ascii', 'ignore' )
			f(output,x,x,data[x])
		output.write( '#\n' )
