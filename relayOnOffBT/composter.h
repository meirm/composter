class COMPOSTER {
public:
  COMPOSTER(int IO_Motor, int IO_Dir,int IO_LoadInterrupt, int IO_DischargeInterrupt);
  void stop();
  void run();
  void restart();
  void runCycle(int numberCycles);
  void stopCycle();
  bool isCycling();
  bool isRunning();
  void stopOnLoad(bool veritas);
  void stopOnDischarge(bool veritas);
  bool stopOnLoad(void);
  bool stopOnDischarge(void);
  void beat();
  void switchDir();
  int getMaxCycles();
  void setMaxCycles(int max);
  int getRemindingCycles();
  unsigned long secsLastSwitch();
  void setTimeOff(int t);
  void setTimeOn(int t);
  int getTimeOn();
  int getTimeOff();
private:
  unsigned long timeOn;
  unsigned long timeOff;
  unsigned long lastSwitch;
  int relay_motor;
  int relay_direction;
  int loadIntIO;
  int disIntIO;
  bool _stopOnLoad;
  bool _stopOnDischarge;
  bool turning;
  bool direction;
  #define right false
  #define left true
  int currCycle;
  int maxcycles;
};