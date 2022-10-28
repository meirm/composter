class COMPOSTER {
public:
  COMPOSTER(int IO_Motor, int IO_LoadInterrupt, int IO_DischargeInterrupt);
  void stop();
  void run();
  void runCycle(int numberCycles);
  void stopCycle();
  bool isCycling();
  bool isRunning();
  void stopOnLoad(bool veritas);
  void stopOnDischarge(bool veritas);
  bool stopOnLoad(void);
  bool stopOnDischarge(void);
  void beat();
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
  int relay;
  int loadIntIO;
  int disIntIO;
  bool _stopOnLoad;
  bool _stopOnDischarge;
  bool turning;
  int currCycle;
  int maxcycles;
};