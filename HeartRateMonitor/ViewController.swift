import UIKit
import CoreBluetooth

let serviceUUID = CBUUID(string: "00001530-1212-EFDE-1523-785FEABCD123")
let service1 = CBUUID(string: "00001532-1212-EFDE-1523-785FEABCD123")
let service2 = CBUUID(string: "00001531-1212-EFDE-1523-785FEABCD123")
let service3 = CBUUID(string: "00001534-1212-EFDE-1523-785FEABCD123")


class HRMViewController: UIViewController {

@IBOutlet weak var LED: UIButton!
    
  var centralManager: CBCentralManager!
  var btPeripheral: CBPeripheral!

  override func viewDidLoad() {
    super.viewDidLoad()
    
    //Create centralManager
    centralManager = CBCentralManager(delegate: self, queue: nil);
  }
    @IBAction func ledButtonPress(_ sender: Any) {
    }
    
}

//First thing executed. Makes sure central manager is powered on, find and connect peripherals.
extension HRMViewController: CBCentralManagerDelegate{
  func centralManagerDidUpdateState(_ central: CBCentralManager) {
    switch central.state{
    case .unknown:
      print("central.state is .unknown")
    case .resetting:
      print("central.state is .resetting")
    case .unsupported:
      print("central.state is .unsupported")
    case .unauthorized:
      print("central.state is .unauthorized")
    case .poweredOff:
      print("central.state is .poweredOff")
    //If powered on, scan for devices (peripheral)
    case .poweredOn:
      print("central.state is .poweredOn")
      centralManager.scanForPeripherals(withServices: nil); //calls function below
    }
}

    //Check if bluetooth module is found. If it is, stop scan and call connect
  func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
    if (peripheral.name == "Adafruit Bluefruit LE D21D"){
      print("Found Adafruit Bluefruit LE D21D")
      btPeripheral = peripheral;
      btPeripheral.delegate = self
      central.stopScan();
      centralManager.connect(btPeripheral)
    }
  }
  
    //Connect to bluetooth module after founding and call discover devices
  func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
    print("Connected!");
    //Passing in an ID makes sure that we only discover one service. CHANGE [serviceUUID] TO nil TO DISCOVER MULTIPLE SERVICES
    btPeripheral.discoverServices([serviceUUID]);
  }
}

//Print services and their characteristics
extension HRMViewController: CBPeripheralDelegate{
  func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
    guard let services = peripheral.services else {return}
    
    //Print list of services
    for service in services{
      print(service)
      peripheral.discoverCharacteristics(nil, for: service)
    }
  }
  
  //Print list of characteristics of the service
  func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
    guard let characteristics = service.characteristics else {return}
    
    for characteristic in characteristics{
      print (characteristics)
      //Check characteristics with read properties
      if characteristic.properties.contains(.read){
        print("\(characteristic.uuid): proprties contains .read")
        peripheral.readValue(for: characteristic)
      }
      //Check characteristics with write properties
      if characteristic.properties.contains(.notify){
        print("\(characteristic.uuid): proprties contains .notify")
      }
    }
  }
  
  //Print value of characteristic with .read property
  func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
    switch characteristic.uuid{
    case service3:
      print(characteristic.value ?? "no value")
    default:
      print("Unhandled Characteristic UUID: \(characteristic.uuid)")
    }
  }
}




