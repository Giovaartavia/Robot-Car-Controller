import UIKit
import CoreBluetooth

//Bluetooth Identifiers:
struct ids{
  let serviceUUID = CBUUID(string: "00001530-1212-EFDE-1523-785FEABCD123")
  let characteristicUUID = CBUUID(string: "6E400002-B5A3-F393-E0A9-E50E24DCCA9E")
  let adafruitIdentifier = UUID(uuidString: "E8F1802E-2A31-3519-783A-D22CEE3FE404")
}

//Global variables used to save string and speed to be sent to Arduino
var temporaryString: [CBCharacteristic]? = nil
var speedValue = 100

class HRMViewController: UIViewController {
    
    
    @IBOutlet weak var sliderLabel: UILabel!
  
    //Declare struct:
    let id = ids();
    
    @IBAction func rightButtonPress(_ sender: Any) {
      let string = "right" + "/" + String(speedValue)
      let data = string.data(using: String.Encoding.utf8)

      for characteristic in temporaryString as! [CBCharacteristic]
      {
        if(characteristic.uuid == id.characteristicUUID)
        {
          btPeripheral.writeValue(data!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
        }
      }
    }
  
    @IBAction func fwdButtonPress(_ sender: Any) {
      let string = "fwd" + "/" + String(speedValue)
      let data = string.data(using: String.Encoding.utf8)
      
      for characteristic in temporaryString as! [CBCharacteristic]
      {
        if(characteristic.uuid == id.characteristicUUID)
        {
          btPeripheral.writeValue(data!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
        }
      }
    }
    
    @IBAction func leftButtonPress(_ sender: Any) {
      let string = "left" + "/" + String(speedValue)
      let data = string.data(using: String.Encoding.utf8)
      
      for characteristic in temporaryString as! [CBCharacteristic]
      {
        if(characteristic.uuid == id.characteristicUUID)
        {
          btPeripheral.writeValue(data!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
        }
      }
    }
    
    @IBAction func backButtonPress(_ sender: Any) {
      let string = "back" + "/" + String(speedValue)
      let data = string.data(using: String.Encoding.utf8)
      
      for characteristic in temporaryString as! [CBCharacteristic]
      {
        if(characteristic.uuid == id.characteristicUUID)
        {
          btPeripheral.writeValue(data!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
        }
      }
    }
    
    @IBAction func fLeftButtonPress(_ sender: Any) {
        let string = "fleft" + "/" + String(speedValue)
        let data = string.data(using: String.Encoding.utf8)
        
        for characteristic in temporaryString as! [CBCharacteristic]
        {
            
            if(characteristic.uuid == id.characteristicUUID)
            {
                btPeripheral.writeValue(data!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
            }
        }
    }
    
    @IBAction func fRigthButtonPress(_ sender: Any) {
        let string = "fright" + "/" + String(speedValue)
        let data = string.data(using: String.Encoding.utf8)
        
        for characteristic in temporaryString as! [CBCharacteristic]
        {
            if(characteristic.uuid == id.characteristicUUID)
            {
                btPeripheral.writeValue(data!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
            }
        }
    }
    
    @IBAction func bLeftButtonPress(_ sender: Any) {
        let string = "bleft" + "/" + String(speedValue)
        let data = string.data(using: String.Encoding.utf8)
        
        for characteristic in temporaryString as! [CBCharacteristic]
        {
            if(characteristic.uuid == id.characteristicUUID)
            {
                btPeripheral.writeValue(data!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
            }
        }
    }
    
    @IBAction func bRightButtonPress(_ sender: Any) {
        let string = "bright" + "/" + String(speedValue)
        let data = string.data(using: String.Encoding.utf8)
        
        for characteristic in temporaryString as! [CBCharacteristic]
        {
            if(characteristic.uuid == id.characteristicUUID)
            {
                btPeripheral.writeValue(data!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
            }
        }
    }
  
  
    @IBAction func stopButtonPress(_ sender: Any) {
        let string = "stop" + "/0"
        let data = string.data(using: String.Encoding.utf8)
        
        for characteristic in temporaryString as! [CBCharacteristic]
        {
            if(characteristic.uuid == id.characteristicUUID)
            {
                btPeripheral.writeValue(data!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
            }
        }
    }
    
    @IBAction func slider(_ sender: UISlider) {
      sliderLabel.text = String(Int(sender.value))
      speedValue = Int(sender.value)
    }
  
  
  //Declare CentralManager and Peripheral
  var centralManager: CBCentralManager!
  var btPeripheral: CBPeripheral!

  override func viewDidLoad() {
    super.viewDidLoad()
    
    //Create centralManager
    centralManager = CBCentralManager(delegate: self, queue: nil)
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
      centralManager.scanForPeripherals(withServices: nil) //calls function below
    }
}

    //Check if bluetooth module is found. If it is, stop scan and call connect
  func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
    //print(peripheral)
    if (peripheral.identifier == id.adafruitIdentifier){
      print("Found Adafruit Bluefruit LE")
      btPeripheral = peripheral
      btPeripheral.delegate = self
      central.stopScan()
      centralManager.connect(btPeripheral)
    }
   // print(peripheral)
  }
  
    //Connect to bluetooth module after founding and call discover devices
  func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
    print("Connected!")
    //Passing in an ID makes sure that we only discover one service. CHANGE [serviceUUID] TO nil TO DISCOVER MULTIPLE SERVICES
    btPeripheral.discoverServices(nil)
  }
}

//Print services and their characteristics
extension HRMViewController: CBPeripheralDelegate{
  func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
    guard let services = peripheral.services else {return}
    
    //Print list of services
    for service in services{
      //print(service)
      peripheral.discoverCharacteristics(nil, for: service)
    }
  }
  
  //Print list of characteristics of the service
  func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
    guard let characteristics = service.characteristics else {return}
    temporaryString = service.characteristics
    
    for characteristic in characteristics{
      //print (characteristics)

      //Check characteristics with write properties
      if characteristic.uuid == id.characteristicUUID {
        let string = "abc" //test string
        let data = string.data(using: String.Encoding.utf8)
        
        peripheral.writeValue(data!, for: characteristic, type: CBCharacteristicWriteType.withResponse)
        
      }
      else if characteristic.properties.contains(.notify){
        btPeripheral.setNotifyValue(true, for: characteristic)
      }
    }
      
      
  }
  
  //Print value of characteristic with .read property
  func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
    if(error != nil){
      print("ERROR")
      return
    }
    
  }
  
  func peripheral(_ peripheral: CBPeripheral, didWriteValueFor characteristic: CBCharacteristic, error: Error?) {
    if (error != nil) {
      print("error \(error)")
      return
    }
    
    print("Message successfuly sent!")
  }
}




