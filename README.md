# arduino-misc
様々なarduinoのスケッチ


## telnet-serial

Arduino(と互換機)をtelnetサーバとし，シリアルでUNIX系のサーバに接続することでコンソール接続をネットワーク越しに行うものです．

## IrRelayRestServer
rest APIへのアクセスで赤外線リモコンのコマンドを飛ばす(Arduinoを赤外線リモコンにする)サーバの実装．IR remoteがサポートしている主要な赤外線リモコンプロトコルの大部分をサポートしているため，多くの機種のリモコンを1つのArduinoにまとめ，そのArduinoをwebサーバとして運用することで，部屋の外からでもArduino経由で電気機器の制御が可能となります．


