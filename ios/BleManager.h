#import "React/RCTBridgeModule.h"
#import "React/RCTEventEmitter.h"
#import <CoreBluetooth/CoreBluetooth.h>


@interface BleManager : RCTEventEmitter <RCTBridgeModule, CBCentralManagerDelegate, CBPeripheralDelegate>{
    NSString* discoverPeripherialCallbackId;
    NSMutableDictionary* connectCallbacks;
    NSMutableDictionary *readCallbacks;
    NSMutableDictionary *writeCallbacks;
    NSMutableDictionary *readRSSICallbacks;
    NSMutableDictionary *retrieveServicesCallbacks;
    NSMutableArray *writeQueue;
    NSMutableDictionary *notificationCallbacks;
    NSMutableDictionary *stopNotificationCallbacks;
    NSMutableDictionary *retrieveServicesLatches;
    // NOTE: My English is pool, so I'll explain it by chinese ( ´▽｀)
    // TODO: 问题: 这个是我在reconnect peripheral时发生的问题，当连接完一个peripheral后，调用了disconnect方法，而后重新调用start、scan方法，扫描后连接相同的peripheral，在通过connect方法调用 retrieveService 会出现非必现的「Peripheral not found or not connected」的错误返回
    // TODO: 探索: 通过断点方式发现调用 connect 方法没问题，触发didConnectPeripheral 也没问题，但是当调用retrieveService时发现通过 findPeripheralByUUID 方法查找到的 peripheral 有概率不是在 didConnectPeripheral 中返回的 peripheral，原谅我不了解iOS开发，故原因不详
    // TODO: 暂时解决：所以加了个 currentConnectedPeripheral 属性，在 didConnectPeripheral 中将其缓存下，在 retriveService 调用 findPeripheralByUUID 方法时，如果有连接优先返回缓存下的 currentConnectPeripheral，为赶工期暂不深究，目前不知道对别的方法有何影响，故写此TODO，静待正统解决方案
    // NOTE: The last connected peripheral. when didDisconnectPeripheral be called, it will reset to nil
    CBPeripheral *currentConnectedPeripheral;
}

@property (strong, nonatomic) NSMutableSet *peripherals;
@property (strong, nonatomic) CBCentralManager *manager;
@property (weak, nonatomic) NSTimer *scanTimer;

// Returns the static CBCentralManager instance used by this library.
// May have unexpected behavior when using multiple instances of CBCentralManager.
// For integration with external libraries, advanced use only.
+(CBCentralManager *)getCentralManager;

// Returns the singleton instance of this class initiated by RN.
// For integration with external libraries, advanced use only.
+(BleManager *)getInstance;

@end
