perl separator_big5.pl corpus.txt > corpus_seg.txt
ngram-count ?text corpus_seg.txt ?write corpus_count.txt ?order 2
ngram-count ?read corpus_count.txt ?lm lan_bigram.txt ?order 2 -unk

make map Big5-ZhuYin ZhuYin-Big5.map

perl separator_big5.pl test_data/1.txt > seg_1.txt
perl separator_big5.pl test_data/2.txt > seg_2.txt
perl separator_big5.pl test_data/3.txt > seg_3.txt
perl separator_big5.pl test_data/4.txt > seg_4.txt
perl separator_big5.pl test_data/5.txt > seg_5.txt
perl separator_big5.pl test_data/6.txt > seg_6.txt
perl separator_big5.pl test_data/7.txt > seg_7.txt
perl separator_big5.pl test_data/8.txt > seg_8.txt
perl separator_big5.pl test_data/9.txt > seg_9.txt
perl separator_big5.pl test_data/10.txt > seg_10.txt
make
./mydisambig seg_1.txt ZhuYin-Big5.map lan_bigram.txt result_1.txt
./mydisambig seg_2.txt ZhuYin-Big5.map lan_bigram.txt result_2.txt
./mydisambig seg_3.txt ZhuYin-Big5.map lan_bigram.txt result_3.txt
./mydisambig seg_4.txt ZhuYin-Big5.map lan_bigram.txt result_4.txt
./mydisambig seg_5.txt ZhuYin-Big5.map lan_bigram.txt result_5.txt
./mydisambig seg_6.txt ZhuYin-Big5.map lan_bigram.txt result_6.txt
./mydisambig seg_7.txt ZhuYin-Big5.map lan_bigram.txt result_7.txt
./mydisambig seg_8.txt ZhuYin-Big5.map lan_bigram.txt result_8.txt
./mydisambig seg_9.txt ZhuYin-Big5.map lan_bigram.txt result_9.txt
./mydisambig seg_10.txt ZhuYin-Big5.map lan_bigram.txt result_10.txt