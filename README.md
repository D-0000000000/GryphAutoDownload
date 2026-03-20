# 电脑自己下载终末地

顾名思义，这个下载器可以给你下载终末地，不过下载的不是启动器，是本体。在1.1更新后肝了几天肝出来了一个非常难用的版本。不排除后面游戏更新后这个下载器就不能用了的可能性。我倒要看看是我先弃坑还是鹰角把这傻了吧唧安装时非要给你下个好几个分卷压缩包然后一起解压占用大量硬盘空间的启动器塞回娘胎里重新出生一遍。

## 功能

下载终末地

优点

* 不需要双倍解压空间。

缺点

* 有点难用
* 在Linux上写的，Windows上的没做
* 目前只支持单线程下载、单线程解压，所以会慢
* 除了游戏文件本身，下载过程中的分卷压缩包都在在内存中，会占用不小的内存
* 重连没有做
* 校验没有做

IT JUST WORK

## 用法

```hgdler manifest_url```

建议下载后通过官方的启动器校验文件，因为我没做校验。

```manifest_url```是用于获取游戏资源的URL,定义如下

### 地址格式

国服 CN ```https://launcher.hypergryph.com/api/${product}/get_latest?appcode=${appcode}&channel=${channel}&sub_channel=${sub_channel}```

国际服 OS ```https://launcher.gryphline.com/api/${product}/get_latest?appcode=${appcode}&channel=${channel}&sub_channel=${sub_channel}```

### product

| product  |
| :------  |
| game     |
| launcher |

### game

| server                  |     appcode      | channel | sub_channel |
| :---------------------- | :--------------- | :------ | :---------- |
| Endfield CN Official    | 6LL0KJuqHBVz33WK | 1       | 1           |
| Endfield CN Bilibili    | 6LL0KJuqHBVz33WK | 2       | 2           |
| Endfield OS Official    | YDUTE5gscDZ229CW | 6       | 6           |
| Endfield OS Epic        | YDUTE5gscDZ229CW | 6       | 801         |
| Endfield OS Google Play | YDUTE5gscDZ229CW | 6       | 802         |
| Arknights CN Official   | GzD1CpaWgmSq1wew | 1       | 1           |
| Arknights CN Bilibili   | GzD1CpaWgmSq1wew | 2       | 2           |

### launcher

| server         |     appcode      | channel | sub_channel |
| :------------- | :--------------- | :------ | :---------- |
| CN Official    | abYeZZ16BPluCFyT | 1       | 1           |
| OS Official    | TiaytKBUIEdoEwRT | 6       | 6           |
| OS Epic        | BBWoqCzuZ2bZ1Dro | 6       | 801         |
| OS Google Play | BBWoqCzuZ2bZ1Dro | 6       | 802         |

注意：国服和国际服的地址不可混用。虽然本下载器仅为终末地使用，但只要```appcode```、```channel```和```sub_channel```匹配，理论上也可以用来下载明日方舟PC端。

### Example

下载国服客户端

```hgdler "https://launcher.hypergryph.com/api/game/get_latest?appcode=6LL0KJuqHBVz33WK&channel=1&sub_channel=1"```

## 依赖

* [libcurl](https://curl.se/libcurl/)下载
* [zlib-ng](https://github.com/zlib-ng/zlib-ng)解压Deflate
* [nlohmann/json](https://github.com/nlohmann/json)解析json
* [spdlog](https://github.com/gabime/spdlog)打印log
* [OpenSSL](https://www.openssl.org/)计算MD5 hash，~~是的，孩子们，虽然我写了计算MD5的部分，但是懒得做出错后重新下载，所以其实没有用~~

## 后续计划

* 加入出错后重新下载
* 加入下载后校验文件
* 自动播放汤汤EP

## Notes

如果你发现你的电脑在自己下载终末地然后找到了我，那你找错地方了
