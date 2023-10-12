# RaspiServer
自宅でサーバーとして常時稼働
```
Raspberry Pi 4 Model B
Debian GNU/Linux 11 (bullseye)
Linux 5.15.84-v8+
```
**詳細説明は各システムのディレクトリ配下に記載**

# 照明操作システム(LightRimocon/配下)
自宅のリモコンの赤外線を学習し、接続した赤外線LEDから部屋の照明を操作するシステム  
常時サーバーを稼働させ、タクトスイッチの押下検知もしくはHTTPリクエストによりプログラム起動

# 天気予報表示システム(WeatherCast/配下)
スクレイピングにより自宅周辺の天気予報情報を取得し、I2Cで接続された電子ペーパーに表示するシステム  
Crontabに登録し定時実行

# Switchbot温湿度計読み取り(Thermometer/配下)
購入したSwitchbotの温湿度計のデータ読み取りプログラム  
冷蔵庫内の温度を測定

# 設置画像
## 照明操作システム 配線
中央にある透明のLED電球3つが赤外線を発信  
下部のタクトスイッチで操作  
赤色LED(赤外線LEDのそば)を取り付け、目視で動作が確認できるようにした  
![image](https://github.com/gengithub17/RaspiServer/assets/25129056/76a70cc1-d3a4-4d39-be59-0d78ad9ca2bc)


## 天気予報表示システム
ケーブルを通じてSPI通信でRaspiから情報を受け取る
![image](https://github.com/gengithub17/RaspiServer/assets/25129056/40d9318d-6b2b-4237-bcd5-26dd39f3a2cd)

## 全体図
赤外線LEDが気まぐれなのか、たまに角度を変えないと照明に信号が届きません
![image](https://github.com/gengithub17/RaspiServer/assets/25129056/6d6a64d6-5d54-42a0-ab86-40d8838d9fdb)

