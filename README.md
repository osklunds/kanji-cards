
# kanji-cards

This project generates cards aimed for studying Kanji. Each card contains

- The Kanji in question
- The number of the Kanji, in order of (approximate) frequency of occurrence
- Stroke order diagrams, showing how to write the Kanji
- Meanings of the Kanji
- On'yomi readings of the Kanji
- Kun'yomi readings of the Kanji
- Words containing the Kanji, in order of (approximate) frequency of occurrence, including the word in kanji, the word's reading and the word's meanings

See the below example of two cards.

<p align="middle">
  <img src="doc/hi.jpg" width="45%" />
  <img src="doc/hon.jpg" width="45%" /> 
</p>

The cards are PDF files. They have a shape aimed at being looked at from a smartphone. See the section `Building, running, testing` for information on how to generate the cards.

This project uses some external data sources and fonts. See the sections `External data` and `Copyright, license, attribution` for more information.

# Source code overview

The "main" function of this program is the test case `run_main` located in `create_card_tests.cpp`. `run_main` in turn uses the other files and classes. See each header file for more details about each file.

# Building, running, testing

Run `./build_run_test.sh` located in the root level of this repository to, well, build the project and run the tests. Run `./build_run_test run_main` to run the main function that generates the kanji cards. The output is located in `build/out`.

# External data and acknowledgments

This project uses external data sources the build the cards. A big thank you to them!

- [JMdict_e](https://www.edrdg.org/wiki/index.php/JMdict-EDICT_Dictionary_Project) for information on words. Checked in at `ext_assets`.
- [kanjidic2](https://www.edrdg.org/wiki/index.php/KANJIDIC_Project#Content_&_Format) for information on kanji. Checked in at `ext_assets`.
- [KanjiVG](https://kanjivg.tagaini.net/) for stroke order diagrams. Checked in at `ext_assets`.
- [2242 KANJI FREQUENCY LIST 1.1](https://docs.google.com/spreadsheets/d/1MBYfKPrlST3F51KIKbAlsGw1x4c_atuHfPwSSRN5sLs/edit?pli=1&gid=496425456#gid=496425456) [(and here)](https://www.researchgate.net/publication/357159664_2242_Kanji_Frequency_List_ver_11) for frequency order of Kanji. Downloaded during build time to `build`.
- [Noto Sans Japanese](https://fonts.google.com/noto/specimen/Noto+Sans+JP) as the font used. Checked in at `ext_assets`.

Also, a special thanks to [jisho.org](https://jisho.org/), an awesome online Japanese dictionary that I've drawn inspiration from.

# Copyright, license, attribution

The license for this repository is GPL v3, as specified by the separate `LICENSE` file, with the following two exceptions:

1. Files in `ext_assets`. This project uses assets from external projects. They are stored as copies inside `ext_assets` alongside copyright files. Their copyright notices and attribution are also shown further down in this document. Since they are copies, I don't claim any copyright or ownership over them. I simply redistribute together with this repository to make building, testing and running my code simpler and more self-contained.

2. Files in `doc` todo

No copyright infringement intended. If you see an issue, I'm more than happy to fix it.

## JMdict_e.xml and kanjidic2.xml

This package uses the [JMdict/EDICT](https://www.edrdg.org/wiki/index.php/JMdict-EDICT_Dictionary_Project) and [KANJIDIC](https://www.edrdg.org/wiki/index.php/KANJIDIC_Project) dictionary files. These files are the property of the [Electronic Dictionary Research and Development Group](https://www.edrdg.org/), and are used in conformance with the Group's [licence](https://www.edrdg.org/edrdg/licence.html). 

## kanjivg

Stroke diagrams are based on [KanjiVG](https://kanjivg.tagaini.net/). KanjiVG is copyright © 2009-2024 Ulrich Apel. It is released under the [Creative Commons Attribution-Share Alike 3.0](https://creativecommons.org/licenses/by-sa/3.0/) license. 

## NotoSansJP-VariableFont_wght.ttf

Copyright 2014-2021 Adobe ([http://www.adobe.com/](http://www.adobe.com/)), with Reserved Font Name 'Source'

This Font Software is licensed under the SIL Open Font License, Version 1.1 . This license is copied ~~below~~ in `ext_assets/Noto_Sans_JP.zip.license`, and is also available with a FAQ at: [https://openfontlicense.org](https://openfontlicense.org)

SIL OPEN FONT LICENSE Version 1.1 - 26 February 2007 
