CONTRIB_PATH=`cd "./contribs"; pwd`
cd ./contribs/src
tar -xvf ./libmicrohttpd-0.9.39.tar.gz
cd libmicrohttpd-0.9.39
./configure --prefix $CONTRIB_PATH
make install
cd ..
rm -rf libmicrohttpd-0.9.39
unzip gsoap_2.8.30.zip
cd gsoap-2.8
./configure --disable-ssl --prefix $CONTRIB_PATH
make install
cd ..
rm -rf curl-7.42.1
tar -xvf ./curl-7.42.1.tar.gz
cd curl-7.42.1
./configure --prefix $CONTRIB_PATH
make install

