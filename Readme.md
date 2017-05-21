# 基于向量余弦的文件相似度计算

## 项目结构

```
.
├── Doc_sim_V0.1              #初始版本
│   ├── Makefile
│   ├── Test_set			  #测试集
│   │   ├── baidu_trans
│   │   ├── baidubaike
│   │   ├── doc1
│   │   ├── doc1.docx
│   │   ├── doc1_fake
│   │   ├── doc2
│   │   ├── doc2_fake
│   │   ├── google_trans
│   │   ├── test.docx
│   │   └── wikipedia
│   ├── sim
│   ├── sim.c
│   ├── sim.py
│   └── sim_eng.c
├── Doc_sim_V0.2                #第二版本
│   ├── Makefile
│   ├── Test_set				#测试集
│   │   ├── baidu_trans
│   │   ├── baidubaike
│   │   ├── doc1
│   │   ├── doc1.docx
│   │   ├── doc1_fake
│   │   ├── doc2
│   │   ├── doc2_fake
│   │   ├── google_trans
│   │   ├── test.docx
│   │   └── wikipedia
│   ├── dir.c
│   ├── sim
│   ├── sim.c
│   ├── sim.py
│   └── sim_eng.c
└── Readme.md
```



## 环境

本人测试环境为

```
$ python -V
Python 2.7.10
$ gcc -v
Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/usr/include/c++/4.2.1
Apple LLVM version 8.1.0 (clang-802.0.42)
Target: x86_64-apple-darwin16.5.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
$
```



## 依赖库

读写`Microsoft Word`需要`Python-docx`库的支持。用下面命令安装：

```
pip install python-docx
```



## 编译

- 编译生成`N-grams`为

  ```
  make sim
  ```

- 编译生成英文分词为

  ```
  make sim_eng
  ```

生成的应用都是`sim`，**注意：在这两种之间切换时，一定要先`make clean`**



## 使用方法

- 直接用编译生成的`sim`可以对非`Microsoft word`即`.doc` `.docx`的文件进行相似度计算

  ```
  $ ./sim
  Usage:./sim [path] or ./sim [file1] [file2]
  $ ./sim ./Test_set/doc1 ./Test_set/doc2
  Similarity between ./Test_set/doc1 and ./Test_set/doc2 is 0.144338
  $ ./sim ./
  Open path: ./
  Read file ./Makefile.....Generate profile success!
  Read file ./sim.....Generate profile success!
  Read file ./sim.c.....Generate profile success!
  Read file ./sim.py.....Generate profile success!
  Read file ./sim_eng.c.....Generate profile success!
                        Makefile            sim          sim.c         sim.py      sim_eng.c
         Makefile       1.000000       0.000000       0.000000       0.000000       0.000000
              sim       0.000000       1.000000       0.000727       0.000000       0.064282
            sim.c       0.000000       0.000727       1.000000       0.000000       0.131036
           sim.py       0.000000       0.000000       0.000000       1.000000       0.000000
        sim_eng.c       0.000000       0.064282       0.131036       0.000000       1.000000
  $
  ```

- 在有依赖库`python-docx`时，可以用`python`脚本对包括`Microsoft word`即`.doc` `.docx`的所有文件进行相似度计算

  ```
  $ python ./sim.py ./Test_set/doc1.docx ./Test_set/doc1
  ./sim ./Test_set/doc1.py_tmp_trans ./Test_set/doc1
  Similarity between ./Test_set/doc1.doc and ./Test_set/doc1 is 1.000000
  $ python ./sim.py ./
  Open path: ./
  Read file ./Makefile.....Generate profile success!
  Read file ./sim.....Generate profile success!
  Read file ./sim.c.....Generate profile success!
  Read file ./sim.py.....Generate profile success!
  Read file ./sim_eng.c.....Generate profile success!
                        Makefile            sim          sim.c         sim.py      sim_eng.c
         Makefile       1.000000       0.000000       0.000000       0.000000       0.000000
              sim       0.000000       1.000000       0.000727       0.000000       0.064282
            sim.c       0.000000       0.000727       1.000000       0.000000       0.131036
           sim.py       0.000000       0.000000       0.000000       1.000000       0.000000
        sim_eng.c       0.000000       0.064282       0.131036       0.000000       1.000000
  ```

  ​

## 其他

- 在`N-word`英文词切分计算的时候，可以多个词划分，也可以单个词划分。在单个词的时候就是词频相似度计算，通过`sim_eng.c`中的宏进行修改：

  ```
  #define N_word          1
  ```

- 由于`profile`是保存在内存中的，设置目录下最多有`20`个文件

- 详细设计请见`Documents`目录下的`基于向量余弦的文件相似度计算.pdf`




## LICENSE

WTFPL