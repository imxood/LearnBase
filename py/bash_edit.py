import os

path="/home/peak/develop/git/StudyNote/test/py"

for root,dirs,files in os.walk(path): # 完全路径, 目录名, 文件列表
    for name in files:
        #print name
        if name.endswith(".cc"):
            print root,dirs,name
            filename=root+"/"+name
            f=open(filename,"r")
            filecontent=""
            line=f.readline()
            while line:
                l=line.replace("#include \"??\"","/arcgisapi")
                filecontent=filecontent+l
                line=f.readline()
            f.close()
            # f=file(filename,"w")
            # f.writelines(filecontent)
            # f.close()
