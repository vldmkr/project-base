#ifndef DISPLAY_CONTROL_H
#define DISPLAY_CONTROL_H

SSD1306 m_Display(0x3C, 2, 0);

#define mRowMax 16
#define ROW_FIT_COUNT 8 // HEIGHT/FONT_SIZE
#define CHARS_IN_ROW 16 // WIDTH/FONT_SIZE

uint8_t mCurRowBuf[CHARS_IN_ROW + 1]; // + \0
uint8_t mCurRowCharPos = 0;

uint8_t mScreen[mRowMax][CHARS_IN_ROW + 1]; // + \0
uint8_t mRealRowCount, mCurRowPos;

void clearScreen(void){
  uint8_t i, j;
  for (i = 0; i < mRowMax; i++){
    for (j = 0; j < CHARS_IN_ROW; j++){
      mScreen[i][j] = 0;
    }
  }
}

void addLineToScreen(void){
  uint8_t i, j;
  for (j = 0; j < CHARS_IN_ROW; j++){
    for (i = 0; i < mRowMax - 1; i++){
      mScreen[i][j] = mScreen[i + 1][j];
    }
  }
  for (j = 0; j < CHARS_IN_ROW; j++){
    mScreen[mRowMax - 1][j] = mCurRowBuf[j];
  }
}

void draw(void){  
  for (uint8_t i = 0; i < ROW_FIT_COUNT; ++i){
    m_Display.drawString(0, i*FONT_SIZE, (char *)(mScreen[i + mCurRowPos]));
  }
}

void redraw(void){
  m_Display.clear();
  draw();
  m_Display.display();
}

void execLine(void){
  addLineToScreen();
  mCurRowPos = mRealRowCount; // sroll to the last row
  mCurRowBuf[mCurRowCharPos = 0] = '\0';
  redraw();
}

void charToLine(uint8_t c){
  mCurRowBuf[mCurRowCharPos] = c;
  mCurRowBuf[++mCurRowCharPos] = '\0';
}

void printlnDisplay(const char* str){
  size_t len = strlen(str);
  for (uint8_t i = 0; i < len; ++i){
    if (!(mCurRowCharPos < CHARS_IN_ROW)){
      execLine();
    }
    charToLine(str[i]);
  }
  execLine();
}

void printlnDisplayPGM(const char* PSTR_str){
  size_t len = strlen_P(PSTR_str);
  for (uint8_t i = 0; i < len; ++i){
    if (!(mCurRowCharPos < CHARS_IN_ROW)){
      execLine();
    }
    charToLine(pgm_read_byte_near(PSTR_str + i));
  }
  execLine();
}

void incRow(void){
  if (mCurRowPos < mRealRowCount){
    mCurRowPos++;
    redraw();
  }
}

void decRow(void){
  if (mCurRowPos > 0){
    mCurRowPos--;
    redraw();
  }
}

void initDisplay(boolean rot180 = false){
  m_Display.init();
  m_Display.clear();
  m_Display.display();
  if (rot180){
    m_Display.flipScreenVertically();
  }

  m_Display.setFontScale2x2(false);

  mCurRowPos = mRealRowCount = mRowMax - ROW_FIT_COUNT;

  clearScreen();
}

#endif