#coding=Big5

import sys

if __name__ == "__main__":
    zhu_to_big5 = {}
    for line in open(sys.argv[1], 'rb'):
        
        #print(line.split(b' '),line.split(b' ')[1].decode('big5hkscs'))
        ##[b'\xed\xdb', b'\xa3\xa6\xa3\xab\xa3\xbe\n']  ㄕㄚˇ
        sp = line.split(b' ')
        chinese = sp[0]
        sp = sp[1].split(b'/') #polyphones 
        for zhu_s in sp:
            #print(zhu_s[:2],zhu_s[:2].decode('big5hkscs') ) 
            ##b'\xa3{' ㄌ
            zhu_to_big5[zhu_s[:2]] = [chinese] if zhu_s[:2] not in zhu_to_big5.keys() else zhu_to_big5[zhu_s[:2]]+ [chinese]
    #print(zhu_to_big5[b'\xa3\xac'][1].decode('big5hkscs'))
    ##喔
    #print(zhu_to_big5)
    with open(sys.argv[2], 'w', encoding = "big5hkscs") as f:
        for key in zhu_to_big5.keys():
            zhu_to_big5[key] = list(set(zhu_to_big5[key]))
            f.write(key.decode('big5hkscs')+"  ")
            #print(key.decode('big5hkscs'), end = '')
            for item in zhu_to_big5[key]:
                f.write(item.decode('big5hkscs')+ " ")
                #print(item.decode('big5hkscs'), end = '')
            f.write('\n')
            #print('')
            
            for item in zhu_to_big5[key]:
                f.write(item.decode('big5hkscs')+"  "+ item.decode('big5hkscs')+" \n")
                #print(item.decode('big5hkscs')+" "+ item.decode('big5hkscs'))