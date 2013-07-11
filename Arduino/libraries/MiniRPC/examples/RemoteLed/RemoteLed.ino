#include <MiniRPC.h>
#include <StreamWrapper.h>
#include <StateMachine.h>
#include <BufferManager.h>
#include <Arduino.h>

/*
 * Extend a class from the MiniRPCMethod.
 * Set the rpc method name for the class.
 * And implement prepare() and execute() methods
 * 
 */
class RemoteControlledLed : public MiniRPCMethod
{
  /*
   * constructor, prepare and execute methods
   * must be public for the dispatcher to be called
   */
public:
  // constructor
  RemoteControlledLed(){
    //this rpc method will be called with: led(...)
    setName("led");
    led = 11;
  }
  /*
   * In prepare we get the parameters in the order which we expect them from the stream.
   * this rpc method can be called with for example: led(11, 230)
   * 
   */
  virtual void prepare(){
    get(led);
    get(intensity);
  }
  /*
   * This method is called as soon as all parameters are succesfully read from the stream
   */
  virtual void execute(){
    pinMode(led, OUTPUT);
    analogWrite(led, intensity);
  }
  
  /*
   * The error method is optional
   * There is a default error message if you don't override this method
   */
  virtual void error(){
    Serial.println("Error in parsing arguments");
    Serial.println("Expected: led(<pin>, <intensity>)");
  }
    
  /*
   * don't forget to declare the parameters we are using
   */
private:
  int intensity;
  int led;
};

/*
 * create a stream wrapper and
 * create a dispatcher with it
 * The StreamWrapper can be subclassed to wrap types of streams
 * that don't extend Stream like for example the String class
 */
StreamWrapper in = StreamWrapper();
MiniRPCDispatcher d1 = MiniRPCDispatcher(&in);

/*
 * create an instance of our RPCMethod class 
 */
RemoteControlledLed tm;

void setup()
{
  Serial.begin(9600);
  /*
   * we are using Arduino's serial in this example
   * but we could assign any Stream derived class to the
   * stream wrapper. 
   */
  in.setStream(&Serial);
  /*
   * Don't forget to register your RPCMethod instance
   * The maximum of RPCMethods that a dispatcher can register
   * is set with MAX_METHOD_COUNT in MiniRPC.h
   */
  d1.registerMethod(&tm);
}

void loop(){
  /*
   * trigger the dispatcher
   * If the dispatcher reads a -1 from the stream (which means currently no data available)
   * it stops processing. So update has to be called again to continue, even if the dispatcher
   * was in the middle of parsing an argument.
   */
    d1.update();
}