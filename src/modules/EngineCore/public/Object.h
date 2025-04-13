#ifndef OBJECT_H
#define OBJECT_H

class Object
{
public:
    virtual ~Object()
    {
        Invalidate();
    }
public:
    virtual bool IsValid() const { return isValid; }
    void Invalidate() { isValid = false; }

private:
    bool isValid{true};

};

#endif // OBJECT_H