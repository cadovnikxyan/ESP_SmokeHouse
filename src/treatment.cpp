#include "treatment.h"

DECLARATE_INSTANCE(HeatTreatmentThread)
HeatTreatmentThread::HeatTreatmentThread()
   :
     powerControlThread(PowerControlThread::instance()),
     dallasTempThread(DallasTempThread::instance()),
     probeThread(ProbeThread::instance()),
     controller(new ThreadController()),
     pid(nullptr),
     start(0),
     stop(0),
     startingFlag(false),
     processStarted(false),
     setPoint(DRYING_OUT_TEMP)
{
      controller->add(powerControlThread);
      controller->add(probeThread);
      controller->add(dallasTempThread);     
}

void HeatTreatmentThread::setUpPID()
{
   if ( pid != nullptr )
      delete pid;

   pid = new PID(dallasTempThread->getResultTemp(), powerControlThread->getPowerValuePtr(), &setPoint, consKp, consKi, consKd, DIRECT);
   pid->SetOutputLimits(0, OUT_MAX_VALUE_PID);
}

void HeatTreatmentThread::adjustPID()
{
   double gap = abs(setPoint - *dallasTempThread->getResultTemp()); 
   if ( gap < 5 )
      pid->SetTunings(consKp, consKi, consKd);
   else
      pid->SetTunings(aggKp, aggKi, aggKd);
}

void HeatTreatmentThread::startTreatment()
{
   start = time(nullptr);
}

void HeatTreatmentThread::stopTreatment()
{
   stop = time(nullptr);
}

void HeatTreatmentThread::autoMode() 
{
   if( startingFlag && !processStarted)
   {
      startTreatment();
      controller->run();
      if ( GlobalState::instance()->state.mode == AUTO_MODE )
         setUpPID(); 

      powerControlThread->setConvection(true);
      processStarted = true;
   }
   else if ( !startingFlag && processStarted )
   {
      stopTreatment();
   }

   if ( processStarted )
   {
      if (GlobalState::instance()->currentProbeTemp >= DRYING_DONE_TEMP )
      {
         setPoint = FRYING_OUT_TEMP;
         powerControlThread->setAirPump(true);
      }
      else if (GlobalState::instance()->currentProbeTemp >= FRYING_DONE_TEMP )
      {
         setPoint = BOILING_OUT_TEMP;
         powerControlThread->setConvection(false);
         powerControlThread->setAirPump(false);
         powerControlThread->setWaterPump(true);
      }
      else if ( GlobalState::instance()->currentProbeTemp >= FULL_DONE_TEMP )
      {
         processStarted = false;
         startingFlag = false;
      }
      if( pid != nullptr )
      {
         adjustPID();
         pid->Compute();
      }
   }
}

void HeatTreatmentThread::manualMode() 
{
 if( startingFlag && !processStarted )
   {
      startTreatment();
      controller->run(); 
      powerControlThread->setConvection(true);
      processStarted = true;
   }
   else if ( !startingFlag && processStarted )
   {
      stopTreatment();
   }

   if ( processStarted )
   {
      if (GlobalState::instance()->currentProbeTemp >= DRYING_DONE_TEMP )
      {
         powerControlThread->setAirPump(true);
      }
      else if (GlobalState::instance()->currentProbeTemp >= FRYING_DONE_TEMP )
      {
         powerControlThread->setConvection(false);
         powerControlThread->setAirPump(false);
         powerControlThread->setWaterPump(true);
      }
      else if ( GlobalState::instance()->currentProbeTemp >= FULL_DONE_TEMP )
      {
         processStarted = false;
         startingFlag = false;
      }
   }
}

void HeatTreatmentThread::smokingMode() 
{
     powerControlThread->setConvection(true);
     powerControlThread->setAirPump(true);
}

void HeatTreatmentThread::run() 
{
   if ( GlobalState::instance()->state.mode == AUTO_MODE )
      autoMode();
   else if ( GlobalState::instance()->state.mode == MANUAL_MODE )
      manualMode();
   else if ( GlobalState::instance()->state.mode == SMOKING_MODE )
      smokingMode();

   runned();
}

String HeatTreatmentThread::setState(String stringState)
{     
   DynamicJsonDocument root(400);
   deserializeJson(root, stringState);
   GlobalState::instance()->setMode(root["mode"].as<int>());
   GlobalState::instance()->setHeatingMode(root["heatingMode"].as<int>());
   
   GlobalState::instance()->setHeatingState(root["heatingState"].as<bool>());
   powerControlThread->setConvection(root["convectionState"].as<bool>());
   powerControlThread->setAirPump(root["airPumpState"].as<bool>());
   powerControlThread->setWaterPump(root["waterPumpState"].as<bool>());
   powerControlThread->setIgnition(root["ignitionState"].as<bool>());

   setStart(root["started"].as<bool>());
   return getJsonState();
}

String HeatTreatmentThread::getJsonTemps() const
{
   StaticJsonDocument<200> jsonTemp;
   StaticJsonDocument<200> jsonProbe;
   String temp = DallasTempThread::instance()->getJsonTemp();
   String probe = ProbeThread::instance()->getJsonProbeTemp();
   deserializeJson(jsonTemp, temp);
   deserializeJson(jsonProbe, probe);
   StaticJsonDocument<400> root;
   root["temp"] = jsonTemp.as<JsonObject>();
   root["probe"] = jsonProbe.as<JsonObject>();
   String result;
   serializeJson(root, result);
   return result;
}

void HeatTreatmentThread::setStart(bool flag)
{
   startingFlag = flag;
}

bool HeatTreatmentThread::getStartFlag() const
{
   return startingFlag;
}

String HeatTreatmentThread::getCurrentState() const
{
   return getJsonState();
}

String HeatTreatmentThread::getJsonState() const
{
   StaticJsonDocument<400> root;

   root["mode"] = GlobalState::instance()->getMode();
   root["heatingMode"] = GlobalState::instance()->getHeatingMode();

   root["heatingState"] = GlobalState::instance()->getHeatingState();
   root["convectionState"] = GlobalState::instance()->getConvectionState();
   root["airPumpState"] = GlobalState::instance()->getAirPumpState();
   root["waterPumpState"] = GlobalState::instance()->getWaterPumpState();
   root["ignitionState"] = GlobalState::instance()->getIgnitionState();

   probeThread->getTemp();
   dallasTempThread->getJsonTemp();
   root["currentOutTemp"] = GlobalState::instance()->currentOutTemp;
   root["currentProbeTemp"] = GlobalState::instance()->currentProbeTemp;
   root["started"] = getStartFlag();
   String result;
   serializeJson(root, result);
   return result;
}

void HeatTreatmentThread::doCalibrateProbe()
{

}