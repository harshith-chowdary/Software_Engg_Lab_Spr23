class IntAttr : public Attr {
public:
    IntAttr(int value) : val(value) {}

    bool operator== (const Attr& right) override {
        const IntAttr* p = dynamic_cast<const IntAttr*>(&right);
        return p != nullptr && val == p->val;
    }

    Attr* copy() const override {
        return new IntAttr(*this);
    }

    int value() const { return val; }

private:
    int val;
};

class FloatAttr : public Attr {
public:
    FloatAttr(float value) : val(value) {}

    bool operator== (const Attr& right) override {
        const FloatAttr* p = dynamic_cast<const FloatAttr*>(&right);
        return p != nullptr && val == p->val;
    }

    Attr* copy() const override {
        return new FloatAttr(*this);
    }

    float value() const { return val; }

private:
    float val;
};

class StringAttr : public Attr {
public:
    StringAttr(const string& value) : val(value) {}

    bool operator== (const Attr& right) override {
        const StringAttr* p = dynamic_cast<const StringAttr*>(&right);
        return p != nullptr && val == p->val;
    }

    Attr* copy() const override {
        return new StringAttr(*this);
    }

    string value() const { return val; }

private:
    string val;
};