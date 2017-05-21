import docx
import os
import sys

def readDocx(docName):
    fullText = []
    doc = docx.Document(docName)
    paras = doc.paragraphs
    for p in paras:
        fullText.append(p.text)
    return '\n'.join(fullText)

#Gen tmp file for filename
def get_python_trans_file(filename,path):
    doc_loc = filename.find(".doc")
    path_loc = filename.find("/")
    new_file = filename[0:doc_loc]
    new_file += ".py_tmp_trans"
    fullText = []
    fullText = readDocx(path+filename)
    file_object = open(path+new_file, 'w')
    file_object.write(fullText)
    file_object.close()
    return new_file

argc = len(sys.argv)
if argc == 2  :
    path = sys.argv[1]
    if path[len(path)-1] != '/':
        path = path + '/'
    if os.path.isdir(path) == False:
        print("%s is not a Path"%(path))
    else:
        files = os.listdir(path)
        #Gen tmp files for doc document
        for i in range(len(files)):
            if files[i].find(".doc") != -1 :
                get_python_trans_file(files[i],path)
        os.system("./sim %s"%(path))
        #Del tmp files
        files = os.listdir(path)
        for i in range(len(files)):
            if files[i].find(".py_tmp_trans") != -1 :
                os.remove(path+files[i])
elif argc == 3  :
    filename1 = sys.argv[1]
    filename2 = sys.argv[2]
    if os.path.exists(filename1) == False or os.path.exists(filename2) == False:
        print("File not exist!")
        sys.exit(1)
    if filename1.find(".doc") != -1 :
        filename1 = get_python_trans_file(filename1,"")
    if filename2.find(".doc") != -1 :
        filename2 = get_python_trans_file(filename2,"")
    print("./sim %s %s"%(filename1,filename2))
    os.system("./sim %s %s"%(filename1,filename2))
    if filename1.find(".py_tmp_trans") != -1 :
        os.remove(filename1)
    if filename2.find(".py_tmp_trans") != -1 :
        os.remove(filename2)
else :
    print("Usage:Python sim.py [file1] [file2] or Python sim.py [path]");
