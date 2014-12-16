ios objc 方法调用记录插件: itracer v1.2

如果你想逆向 某些app的调用流程 或者 系统app的一些功能的 私有 framework class api 调用流程， 可以试试此工具

只需要 配置需要挂接的 类名和app名， 就可以实时追踪 相关功能的 调用流程。 支持批量 hook n多个类名

特性：
* 批量跟踪ios下指定class对象的所有调用流程
* 支持ios for armv6,armv7,arm64 以及mac for x86, x64
* 自动探测参数类型，并且打印所有参数的详细信息

更新内容：

* 增加对arm64的支持，刚调通稳定性有待测试。
   arm64进程注入没时间做了，暂时用了substrate的hookprocess， 所以大家需要先装下libsubstrate.dylib
   armv7的版本是完全不依赖substrate的。

* arm64的版本对参数的信息打印稍微做了些增强。

注：此项目已不再维护，仅供参考。

1. 配置需要挂接的class

    ```
    修改itrace.xml配置文件，增加需要hook的类名：
    <?xml version="1.0" encoding="utf-8"?>
    <itrace>
      <class>
        <SSDevice/>
        <SSDownload/>
        <SSDownloadManager/>
        <SSDownloadQueue/>
        <CPDistributedMessagingCenter/>
        <CPDistributedNotificationCenter/>
        <NSString args="0"/>
      </class>
    </itrace>
    ```

    注： 尽量不要去hook， 频繁调用的class， 比如 UIView NSString， 否则会很卡，操作就不方便了。
    注： 如果挂接某个class， 中途打印参数信息挂了， 可以在对应的类名后面 加上 args="0" 属性， 来禁止打印参数信息， 这样会稳定点。 
         如果要让所有类都不打印参数信息， 可以直接设置： <class args="0">


2. 安装文件

    将整个itracer目录下的所有文件用手机助手工具，上传到ios系统上的 /tmp 下面：
    ```
    /tmp/itracer
    /tmp/itrace.dylib
    /tmp/itrace.xml
    ```

3. 进行trace

    进入itracer所在目录：
    ```
      cd /tmp
      ```

    修改执行权限：
    ```
      chmod 777 ./itracer
    ```

    运行程序: 

    ```
      ./itracer springboard (spingboard 为需要挂接的进程名， 支持简单的模糊匹配)
    ```

4. 查看 trace log， 注： log 的实际输出在： Xcode-Windows菜单-Organizer-Console 中：

    ```
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownloadQueue downloads]
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownloadManager downloads]
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownloadManager _copyDownloads]
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownloadQueue _sendDownloadStatusChangedAtIndex:]: 0
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownloadQueue _messageObserversWithFunction:context:]: 0x334c5d51: 0x2fe89de0
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownloadQueue downloads]
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownloadManager downloads]
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownloadManager _copyDownloads]
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownload cachedApplicationIdentifier]
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownload status]
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [SSDownload cachedApplicationIdentifier]
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [CPDistributedNotificationCenter postNotificationName:userInfo:]: SBApplicationNotificationStateChanged: {
          SBApplicationStateDisplayIDKey = "com.apple.AppStore";
          SBApplicationStateKey = 2;
          SBApplicationStateProcessIDKey = 5868;
          SBMostElevatedStateForProcessID = 2;
      }
    Jan 21 11:12:58 unknown SpringBoard[5706] <Warning>: [itrace]: [3edc9d98]: [CPDistributedNotificationCenter postNotificationName:userInfo:toBundleIdentifier:]: SBApplicationNotificationStateChanged: {
          SBApplicationStateDisplayIDKey = "com.apple.AppStore";
          SBApplicationStateKey = 2;
          SBApplicationStateProcessIDKey = 5868;
          SBMostElevatedStateForProcessID = 2;
      }: null
    Jan 21 11:12:59 unknown SpringBoard[5706] <Warning>: [itrace]: [105d7000]: [SSDownloadManager _handleMessage:fromServerConnection:]: 0xe6920b0: 0xe007040
    Jan 21 11:12:59 unknown SpringBoard[5706] <Warning>: [itrace]: [105d7000]: [SSDownloadManager _handleDownloadStatesChanged:]: 0xe6920b0
    Jan 21 11:12:59 unknown SpringBoard[5706] <Warning>: [itrace]: [105d7000]: [SSDownloadManager _copyDownloads]
    Jan 21 11:12:59 unknown SpringBoard[5706] <Warning>: [itrace]: [105d7000]: [SSDownload persistentIdentifier]
    Jan 21 11:12:59 unknown SpringBoard[5706] <Warning>: [itrace]: [105d7000]: [SSDownload _addCachedPropertyValues:]: {
          I = SSDownloadPhaseDownloading;
      }
    Jan 21 11:12:59 unknown SpringBoard[5706] <Warning>: [itrace]: [105d7000]: [SSDownload _applyPhase:toStatus:]: SSDownloadPhaseDownloading: <SSDownloadStatus: 0xe6b8e80>
    Jan 21 11:12:59 unknown SpringBoard[5706] <Warning>: [itrace]: [105d7000]: [SSDownloadQueue downloadManager:downloadStatesDidChange:]: <SSDownloadManager: 0x41ea60>: (
          "<SSDownload: 0xe6bd970>: -4085275246093726486"
      )
    ```

donate
------

####alipay
<img src="http://www.tboox.net/ruki/alipay.png" alt="alipay" width="128" height="128">

