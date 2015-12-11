#ifndef CONSOLE_CONTROLLER_H
#define CONSOLE_CONTROLLER_H

#include "dictionary.h"
#include "ConsoleView.h"
#include "Queue.h"

class ConsoleController
{
public:
  ConsoleController(ConsoleView* display);
  void print(unsigned char dictKey);
  void print(String str);
  void execute();

private:
  ConsoleView* mpConsole;
  Queue<unsigned char> mKeyQueue;
  Queue<String> mStrQueue;
};

ConsoleController::ConsoleController(ConsoleView* console): mpConsole(console){
}

void ConsoleController::print(unsigned char dictKey){
  mKeyQueue.push(dictKey);
}

void ConsoleController::print(String str){
  mKeyQueue.push(SERVICE_STRING);
  mStrQueue.push(str);
}

void ConsoleController::execute(){
  if(!mKeyQueue.isEmpty()) {
    unsigned char key = SERVICE_ERROR;
    while((key = mKeyQueue.pop(SERVICE_ERROR)) != SERVICE_ERROR) {
      if(key == SERVICE_STRING) {
        if(!mStrQueue.isEmpty()) {
          mpConsole->println(mStrQueue.pop("").c_str());
        }
      } else {
        mpConsole->printlnPGM(DICTIONARY[key]);
      }
    }
    mpConsole->redraw();
  }
}

#endif //CONSOLE_CONTROLLER_H