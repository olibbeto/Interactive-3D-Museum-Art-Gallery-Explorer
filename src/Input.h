#ifndef INPUT_H
#define INPUT_H

class Input {
public:
    Input();
    
    void setKeyDown(unsigned char key);
    void setKeyUp(unsigned char key);
    bool isKeyDown(unsigned char key) const;

private:
    bool keys[256];
};

#endif // INPUT_H
