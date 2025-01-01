#!/bin/bash

# Script for creating JMdict_e.xml.sample, which contains only a subset of
# the words from the full JMdict_e.xml.sample. If the full data is used too
# many words are found (per kanji) which makes the tests unwieldy.

cp JMdict_e.xml JMdict_e.xml.sample

#          日本$   ,$
sed -i -e '1052066,3555581d' JMdict_e.xml.sample 

#          物語$  ,^日本...
sed -i -e '892255,1052042d' JMdict_e.xml.sample

#          日$  ,^物語
sed -i -e '816829,892226d' JMdict_e.xml.sample 

#          動物$ ,^日
sed -i -e '793369,816780d' JMdict_e.xml.sample 

#          動じる$,^動物
sed -i -e '792895,793349d' JMdict_e.xml.sample 

#          ^  ,動じる^
sed -i -e '522,792873d' JMdict_e.xml.sample 
