from bluepy import btle
import struct
import binascii

# 参考
# https://tsuzureya.net/how-to-use-switchbot-waterproof-thermo-hygrometer/

MAC_ADDR = "cf:f6:43:d4:e0:19"
TIMEOUT = 5.0

#Broadcastデータ取得用デリゲート
class SwitchbotScanDelegate(btle.DefaultDelegate):
    #コンストラクタ
    def __init__(self, macaddr):
        btle.DefaultDelegate.__init__(self)
        self.macaddr = macaddr
        self.temp = None
        self.humid = None
        self.battery = None

    # スキャンハンドラー
    # override
    def handleDiscovery(self, dev, isNewDev, isNewData):
        # 対象Macアドレスのデバイスが見つかったら
        if dev.addr == self.macaddr:
            # アドバタイズデータを取り出し
            for (adtype, desc, value) in dev.getScanData():
                if(adtype == 255): #温湿度データ
                    madata = binascii.unhexlify(value[16:])
                    self.humid = madata[4] & 0b01111111
                    self.temp = (madata[2] & 0b00001111) / 10 + (madata[3] & 0b01111111)
                    isOverZero=(madata[3]&0b10000000)
                    if not isOverZero:
                        self.temp *= -1
                elif(adtype == 22): #バッテリー情報
                    servicedata = binascii.unhexlify(value[4:])
                    self.battery = servicedata[2] & 0b01111111

delegate = SwitchbotScanDelegate(MAC_ADDR)
scanner = btle.Scanner().withDelegate(delegate)
scanner.scan(TIMEOUT)
print(delegate.temp)