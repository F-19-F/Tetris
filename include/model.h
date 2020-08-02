#define PI 3.14
class model
{
    private:
    //方块抽象为8X8方格上相隔1个方块的点的组合
    bool base[8][8];
    //打印用到的临时变量
    //下面为旋转中心的坐标
    int center_x;
    int center_y;
    public:
    bool temp[4][4];
    int length;
    int height;
    //mode表示方块类型，1-5将构建预设类型，0代表自主构建
    model(int mode);
    //输出原始数据，以便调试--for debug
    void print_row(int mode);
    //输出model形状
    void print_model(bool clean);
    //以旋转中心逆时针旋转90度
    void changer_neg(int ang);
    //将原始数据翻译成打印数据
    void get_temp();
};