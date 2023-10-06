# RaspiServer
自宅でサーバーとして常時稼働
```
Raspberry Pi 4 Model B
Debian GNU/Linux 11 (bullseye)
Linux 5.15.84-v8+
```

# 照明操作システム(LightRimocon/配下)
自宅のリモコンの赤外線を学習し、接続した赤外線LEDから部屋の照明を操作するシステム  
常時サーバーを稼働させ、タクトスイッチの押下検知もしくはHTTPリクエストによりプログラム起動

# 天気予報表示システム(WeatherCast/配下)
スクレイピングにより自宅周辺の天気予報情報を取得し、I2Cで接続された電子ペーパーに表示するシステム  
Crontabに登録し定時実行

# Switchbot温湿度計読み取り(Thermometer/配下)
購入したSwitchbotの温湿度計のデータ読み取りプログラム  
冷蔵庫内の温度を測定
