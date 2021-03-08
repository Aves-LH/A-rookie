### PushStream程序用来推流，MediaPlayer程序拉流解析渲染播放
### 运行此程序需要两个终端，一个执行PushStream程序进行推流，另一个执行MediaPlayer程序进行拉流播放

### PushStream程序使用需要指定文件
./PushStream *.mp4

./MediaPlayer


### 如果没有安装ffmpeg和SDL库建议安装后再编译运行此项目
### Makefile文件会有点问题，若安装了ffmpeg和SDL库也可使用build.sh脚本编译此项目
