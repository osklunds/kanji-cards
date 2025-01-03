#!/bin/bash

# Copyright (C) 2025 Oskar Lundström

# This file is part of kanji-cards.

# kanji-cards is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.

# kanji-cards is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along with
# kanji-cards. If not, see <https://www.gnu.org/licenses/>.


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
