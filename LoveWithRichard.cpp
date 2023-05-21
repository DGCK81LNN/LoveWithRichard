//encoding: gbk

#include<iostream>
#include<conio.h>
#include<windows.h>
#include<string>

using namespace std;

/** 
 * @brief 获取控制台光标在第几列
 */
int wherex() {
    CONSOLE_SCREEN_BUFFER_INFO pBuffer;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &pBuffer);
    return pBuffer.dwCursorPosition.X + 1;
}
/**
 * @brief 获取控制台光标在第几行
 */
int wherey() {
    CONSOLE_SCREEN_BUFFER_INFO pBuffer;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &pBuffer);
    return pBuffer.dwCursorPosition.Y + 1;
}
/**
 * @brief 移动控制台光标到指定位置
 */
void gotoxy(int x,int y) {
    COORD c;
    c.X=x-1;
    c.Y=y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
/**
 * @brief 更改输出字体颜色
 * @param a 同color指令的格式，请在cmd中输入help color查看
 */
void color(unsigned char a = 0x07) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
}
/**
 * @brief 判断一个字符串是否是给定数组中的一项
 */
bool inArray(string str, const string *array) {
    int i = 0;
    while (array[i] != "") {
        if (array[i] == str)
            return true;
        i++;
    }
    return false;
}

/**
 * @brief 输出文本，识别字体颜色控制。
 * @param str 要输出的字符串
 * @param end 自定义行尾
 */
void l(string str, string end = "\n\n") {
    color();
    for (int i = 0; str[i] != '\0'; i++) // 设置字体颜色符\x11
        if (str[i] == '\x11')
            color((int)str[++i]);
        else if (str[i] == '\x12') // 重置字体颜色符\x12
            color();
        else
            cout << str[i];
    color();
    cout << end;
}

bool debug = false; // 调试模式是否已开启
string name; // 当前玩家名
float lovePoint; // 好感度
float loveBonus; // 好感度加成倍数

/**
 * @brief 逐字输出文本，识别字体颜色控制。
 * @param str 要输出的字符串
 * @param end 自定义行尾
 */
void p(string str, string end = "\n\n") {
    color();
    for (int i = 0; str[i] != '\0'; i++) // 设置字体颜色符\x11
        if (str[i] == '\x11')
            color(str[++i]);
        else if (str[i] == '\x12') // 重置字体颜色符\x12
            color();
        else {
            if (str[i] != '\x13') // 延时符\x13
                cout << str[i];
            if(str[i] != '\x07' && !debug)
                Sleep(30);
        }
    color();
    cout << end;
    Sleep(150);
}

/**
 * @brief 逐字输出文本，识别字体颜色控制，输出完成后等待按下任意键。
 * @param str 要输出的字符串
 * @param end 自定义行尾
 */
void g(string str, string end = "\n\n") {
    color();
    for (int i = 0; str[i] != '\0'; i++) { // 设置字体颜色符\x11
        if (str[i] == '\x11') {
            color((int)str[++i]);
        }
        else if (str[i] == '\x12') { // 重置字体颜色符\x12
            color();
        }
        else {
            if (str[i] != '\x13') // 延时符\x13
                cout << str[i];
            if(str[i] != '\x07' && !debug)
                Sleep(30);
        }
    }
    color();
    cout << end;
    if ((debug || getch() == 'l') && !debug) {
        int x = wherex(), y = wherey();
        color(0x08);
        gotoxy(1, 1);
        cout << lovePoint << "\a";
        color();
        gotoxy(x, y);
    }
}

/**
 * @brief 输出分割线
 * @param lovePoint 当前的好感度。传入后会显示好感度进度条；如果省略，则显示“按任意键继续”。
 */
void w(float lovePoint = -10000) {
    system("cls");
    p("\n\n\n\n\n\n\n\n\n\n\t\t\t\x11\x0fLove With Richard \x11\x07 under epidemic", "\n");
    if (lovePoint != -10000) {
        cout << "\t\t\t";
        color(0x5c);
        for (float i = 0; i < lovePoint; i += 4.8)
            cout << '\x03';
        p("");
        color();
    }
    else
        p("\t\t\t按任意键继续");
    g("\n\n\n\n\n\n\n\n\n");
}

/**
 * @brief 显示选择题
 * @param choiceArr 选项数组，用一个空字符串结束
 * @return 选中项的索引
 */
unsigned char choices(const string *choiceArr) {
    char g; // 存储最近输入的1字节数据
    unsigned char i = 0; // 当前选中项的索引
    int x = wherex(), y = wherey(); // 存储选择题开始位置的光标坐标
    unsigned char size = 255; // 存储选项数组的长度
    for (unsigned char j = 0; j < size; j++) {
        if (choiceArr[j] == "") {
            size = j;
            break;
        }
        p("\x11\x0f* " + choiceArr[j], "\t\t");
    }
    while (true) {
        gotoxy(x, y);
        for (unsigned char j = 0; j < size; j++) {
            color(i == j ? 0x0e : 0x0f);
            cout << "* " + choiceArr[j] + "\t\t";
        }
        g = getch();
        if (g == '\xe0') {
            g = getch();
            if (g == 'K') {
                if (i > 0)
                    i--;
            }
            else if (g == 'M') {
                i++;
                if (i >= size)
                    i = size - 1;
            }
        }
        else if (g == 'd') {
            debug = !debug;
            cout << (debug ? "\a\a" : "\a");
        }
        else if (g == 'l') {
            int x = wherex(), y = wherey();
            color(0x08);
            gotoxy(1, 1);
            cout << lovePoint << "\a";
            color();
            gotoxy(x, y);
        }
        else if (g == '\r') {
            color();
            l("");
            return i;
        }
    }
}


const string NAMES_AMANDA[] = {"Amanda", "Lxm", ""}; // 判定为Amanda的名字数组
const string NAMES_NORA[] = {"Nora", "Wsy", ""}; // 判定为Nora的名字数组
const string NAMES_SUNNY[] = {"Sunny", "Wxr", "Huuns", ""}; // 判定为Sunny的名字数组
const string NAMES_SUNNYLIKE[] = {"Ynnus", "Suny", "Rxw", ""}; // 判定为形似Sunny的名字数组
bool amanda; // 是否已判定为Amanda
bool nora; // 是否已判定为Nora
bool sunny; // 是否已判定为Sunny
bool sunnylike; // 是否已判定为形似Sunny

// 键盘按键列表
const string CHOICES_KEYBOARD[] = {
    "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N",
    "O", "P", "Q", "R", "S", "T", "U",
    "V", "W", "X", "Y", "Z", "删除", "完成", ""
};

/**
 * @brief 提示输入名字
 * @return 选中项的索引
 */
string keyboard() {
    char g; // 存储最近输入的1字节数据
    unsigned char i = 0; // 当前选中按键的索引
    int x = wherex(), y = wherey(); // 存储键盘开始位置的光标坐标
    unsigned char size = 255; // 存储键盘按键的数量
    unsigned char pos = 0; // 存储键盘光标目前在第几个字节上
    char n[] = "\0\0\0\0\0\0"; // 存储当前已输入的名字
    unsigned char nameColor; // 存储当前名字的字体颜色，0x0到0xf
    for (unsigned char j = 0; j < size; j++) {
        if (CHOICES_KEYBOARD[j] == "") {
            size = j;
            break;
        }
    }
    while (true) {
        amanda = inArray(n, NAMES_AMANDA),
        nora = inArray(n, NAMES_NORA),
        sunny = inArray(n, NAMES_SUNNY),
        sunnylike = inArray(n, NAMES_SUNNYLIKE);
        gotoxy(x, y);
        nameColor = amanda ? 0xa : nora ? 0xd : sunny ? 0xc : sunnylike ? 0xe : 0xf;
        color(nameColor); // 低四位为指定颜色，高四位为0
        cout << "\t\t\t     ";
        for (unsigned char k = 0; k <= pos && k < 6; k++) { // 每个字母挨个输出
            if (k == 5 || k == pos) // 当前光标位置
                color(nameColor << 4); // 低四位为0，高四位为指定颜色
            cout << n[k]; // 如果光标所在的这一位还是空的，正好输出一个空字符\0代表光标
        }
        color();
        cout << "     \n\t";
        for (unsigned char j = 0; j < size; j++) {
            if (j > 0 && j % 7 == 0)
                cout << "\n\t";
            color((i == j ? 0x0e : 0x0f) - (j >= 26 && pos == 0 ? 8 : 0));
            cout << CHOICES_KEYBOARD[j] << '\t';
        }
        color();
        g = getch();
        if (g == '\xe0') {
            g = getch();
            if (g == 'K')
                if (i > 0)
                    i--;
                else
                    i = size - 1;
            else if (g == 'M')
                if (i < size - 1)
                    i++;
                else
                    i = 0;
            else if (g == 'H' && i >= 7)
                    i -= 7;
            else if (g == 'P' && i < size - 7)
                    i += 7;
        }
        else if (g == 'd') {
            debug = !debug;
            cout << (debug ? "\a\a" : "\a");
        }
        else if (g == '\r')
            if (i == 26) {
                if (pos == 0)
                    continue;
                pos--;
                n[pos] = 0;
            }
            else if(i == 27) {
                if (pos == 0)
                    continue;
                color();
                cout << endl;
                return string(&n[0]);
            }
            else {
                if (pos > 5)
                    pos = 5;
                n[pos] = pos == 0 ? 65 + i : 97 + i;
                pos++;
            }
    }
}

int main() {

    system("title Love With Richard v2.0.4");

    // 主菜单

    const string CHOICES_MAINMENU[] = {"开始", "关于", "退出", ""};
    const string CHOICES_BACK[] = {"返回", ""};

    restart:
    l("\n\x11\x0f      |     \x11\x0c/\\ /\\\x11\x0f l     j  -----   l        j --- -------  |     |\n      |    \x11\x0c|##V##|\x11\x0f l   j  |_____    l      j   |     |     |_____|\n      |     \x11\x0c\\###/\x11\x0f   l j   |          l /\\ j    |     |     |     |\n      |____   \x11\x0cv\x11\x0f      V    |_____      V  V    _|_    |     |     |\n\n         ____     ___      ____            _     ____    ____\n        |     \\    |     /      |     |  /   \\  |     \\ |     \\\n        |      )   |    |       |_____| |     | |      )|      |\n        |_____/    |    |       |     | |-----| |_____/ |      |\n        |     \\    |    |       |     | |     | |     \\ |      |   \x11\x07under\n\x11\x0f        |      |  _|_    \\_____ |     | |     | |      ||_____/\x11\x07 epidemic\n\n\n");
    Sleep(1000);
    l("按方向键移动，按回车选定");
    back:
    switch(choices(CHOICES_MAINMENU)) {
        case 1:
            p("\t\t\t\x11\x0fLove With Richard \x11\x07 under epidemic\n\n\n\n\t\t原作（Python 3） by:\n\t\t\t\t\x11\x08 学了一点Python的\n\t\t\t\t     \x11\x0eRichard M\x12\n\t\t    （Bilibili \x11\x09@天府灵山行者\x12 , \x11\x09uid300711293\x12）\n\n\n\t\tC++ 翻制 by:\n\t\t\t\t\x11\x08  除了C++啥都会的\n\t\t\t\t       \x11\x0aRoy L\x12\n\t\t      （Bilibili \x11\x09@DGCK81LNN\x12 , \x11\x09uid328066747\x12）\n\t\t\t     \x11\x02（胡说，我根本不会Java\n\n\n\x11\x08调试功能：在任意选项处按[D]切换调试模式，可以自动跳过对话；在任意处按[L]可在控制台第一行显示当前好感度。");
            choices(CHOICES_BACK);
            goto back;
        case 2:
            return 0;
    }

    // 初始化

    lovePoint = 0; // 好感度

    // 输入名字

    const string CHOICES_CONFIRM[] = {"取消", "确定", ""};

    p("\n你的名字。");
    while (true) {
        name = keyboard();
            l("> " + name + " <");
        if (sunny) {
            p("\x11\x0c我觉得布星");
        }
        else if (sunnylike) {
            p("\x11\x0e彳亍口巴");
            if (choices(CHOICES_CONFIRM) == 1)
                break;
        }
        else if (amanda) {
            p("\x11\x0a你确定？");
            loveBonus = 1.2;
            if (choices(CHOICES_CONFIRM) == 1)
                break;
        }
        else if (nora) {
            p("\x11\x0d你确定？");
            loveBonus = 1.2;
            if (choices(CHOICES_CONFIRM) == 1)
                break;
        }
        else {
            p("是这个名字吗？");
            loveBonus = 1;
            if (choices(CHOICES_CONFIRM) == 1)
                break;
        }
    }
    w();

    // 00 - 故事背景

    g("经过两个多月的网课，疫情基本结束，学校终于要复课了。");
    g("2020.4.20。复课的第一天，我满怀期待地走进了校门。熟悉的景象在我眼前展开，只是所有人都戴了个口罩。顶着大风，仿佛回到了二三月份。");
    g("走在我前面的是我的8B班同学Richard。三个多月没看见他，他在我眼中更加高大了。我在网课时曾屡次想念过他，现在，我可以以学习为理由来接近他，和他在一起。");
    g("我在上个学期观察过他，他很少和Sunny说话，Sunny也没有去主动找他。但我猜不出他是否已经放弃了Sunny。机不可失时不再来，我得在结课考之前追求到他。");
    w(lovePoint);

    // 01 - 重回校园

    const string CHOICES_START[] = {"我有一道题不会", "你好像不太高兴", "我想和你一起复习", ""};
    const string CHOICES_LOVE_RUSH[] = {"对他表白", "回自己的座位", ""};
    const string CHOICES_LOVE_RUSH_R[] = {"我没有复习材料", "对他表白", "回自己的座位", ""};
    bool loveRush = false; // 是否贸然表白

    g("我一边跟在他后面，一边思考要对他说什么。我和他按照老师的引导进入了七年八班的教室，也就是两年前我们上课的地方。");
    g("他擦了自己的桌椅，然后拿出一本必背古诗文，趴在桌子上看。");
    p("\x11\x0f* (要对他说什么？)");
    switch (choices(CHOICES_START)) {
    case 0:
        lovePoint += loveBonus * 10;
        g("我慢慢地向Richard走过去，带着一道我假装不会的题，向他提问：“我有一道数学题不会，这张卷子的第24题，你能给我讲一讲吗？”");
        g("他似乎被吓了一下，抬起头来，惊讶地看着我。他接过了我手中的卷子，看了十几秒，然后开始给我讲。");
        g("“先代值算出它的解析式，再把图像画出来，对P点的位置分三段讨论......”。他讲得极为认真，似乎已经进入了他最为专心致志的境界。");
        g("我有些搞不懂，他对我讲题为什么会这么认真，于是我准备问他一下，这时，他讲完了，又装作平静地对我说：“听明白了么，"+name+"？”");
        g("我本来会这道题，但我回答了：“没有。”他又更加细致的讲了一遍，还把思路都告诉了我。这下，我对他更加崇拜与好奇了。");
        g("他好暖啊。我好奇地问了他一下：“你为什么这么喜欢认真地给我讲题？”");
        g("“我...就是喜欢帮助别人，不只是你。两年前，我就想给别人讲题，没有人来问我。现在，我更加珍惜给别人讲题的机会”他在回答我时眼神在打转，似乎不敢长时间直视我，他有些害羞了。");
        g("我仔细的想了一想，这种不好意思绝对是两年前Sunny给他留下的后遗症。这么一想，他应该幻想过给喜欢的人讲题。");
        g("快上课了，我赶紧扭头走回了自己的座位。");
        break;
    case 1:
        lovePoint += loveBonus * 4;
        g("我慢慢地向Richard走过去，对他说：“你好像有点不高兴，怎么了？”");
        g("“没事，我没事。你去复习你的东西吧，别管我”他抬头看了我一下，然后又低下头，拿起笔开始补作业。");
        g("我有些失望，又问了他一下：“和我谈一谈你的心事行吗？我不会告诉别人。”");
        p("“我没有什么心事，只是学习压力大。”他有些冷漠地回答了我。");
        switch (choices(CHOICES_LOVE_RUSH)) {
        case 0:
            lovePoint -= (1.4 - loveBonus) * 60;
            loveRush = true;
            g("我慢慢地走向他，然后弯下腰把头贴在他的耳边，表白的话已经到了嘴边。");
            g("“我好喜欢你，Richard。”");
            g("“"+name+"，你...你想对我干什么？”他有些不知所措，扭过头去背对着我。");
            g("“"+name+"，我现在不想突然开始和女生交往。马上就要考试了，现在不是干这种事的时候。我还没复习呢。”他就这样拒绝了我。");
            g("我又问了一下他：“你为什么不愿和我交往？我真的很喜欢你。”");
            g("他叹了一口气，低声回答我：“哎——我已经失去一位挚爱了，经历过撕心裂肺的痛苦。我现在承受不起这种刺激。只要我不再和女生交往，就不会爱上，更不会再次经历痛苦的失去。”");
            g("“简单点来说，我就是患上了‘疫后综合征’：怀念疫前的日子，熬过着现在的生活，对未来失去希望。靠回忆度日，虽然平淡无聊，但也不会起大风浪。”");
            g("我赶紧开导他：“我向你保证，我到死也不会离你远去。我肯定能实现你对恋爱的期待，治愈你的‘疫后综合征’”");
            g("他并没有回答我什么，只是一直盯着古诗文小本看。我能肯定他还在怀念和Sunny在一起的日子。");
            g("我的强推战术失败了，可我还是不甘心。他居然对这么可爱的女孩子没有感觉。我有些失望地回了自己的座位。");
        case 1:
            g("我快速遛回了自己的座位，然后我趁他低头复习时偷偷回头看了看他。");
        }
        break;
    case 2:
        lovePoint += loveBonus * 6;
        g("我慢慢地向Richard走过去，对他说：“我想和你一起复习，行吗？”");
        p("他惊讶地抬了起头，向我询问道：“你怎么了？没带卷子还是别的情况？”他的语气中带着一丝冷漠与低迷。");
        switch (choices(CHOICES_LOVE_RUSH_R)) {
        case 0:
            lovePoint += loveBonus * 2;
            g("“没带古诗词小本。我能和你一起看么？”我为接近他，向他撒了个谎。");
            g("“没问题，一起看吧。”他平平地说。");
            g("我走了过去，蹲在他旁边假装在认真地背诵。我缓慢地挪动自己的身体，想要贴在他身上。");
            g("他向左一挪，避开了我的小身躯，仍然目不转睛地盯着小本，完全没看我。他似乎又在思考什么。");
            g("我不敢太张扬地追求，只好享受当下。和他靠的这么近，我感到自己快要融化了。过了一小会儿，我红着脸回了自己的座位。");
            break;
        case 1:
            lovePoint -= (1.4 - loveBonus) * 60;
            loveRush = true;
            g("我慢慢地走向他，然后弯下腰把头贴在他的耳边，表白的话已经到了嘴边。");
            g("“我好喜欢你，Richard。”");
            g("“"+name+"，你...你想对我干什么？”他有些不知所措，扭过头去背对着我。");
            g("“"+name+"，我现在不想突然开始和女生交往。马上就要考试了，现在不是干这种事的时候。我还没复习呢。”他就这样拒绝了我。");
            g("我又问了一下他：“你为什么不愿和我交往？我真的很喜欢你。”");
            g("他叹了一口气，低声回答我：“哎——我已经失去一位挚爱了，经历过撕心裂肺的痛苦。我现在承受不起这种刺激。只要我不再和女生交往，就不会爱上，更不会再次经历痛苦的失去。”");
            g("“简单点来说，我就是患上了‘疫后综合征’：怀念疫前的日子，熬过着现在的生活，对未来失去希望。靠回忆度日，虽然平淡无聊，但也不会起大风浪。”");
            g("我赶紧开导他：“我向你保证，我到死也不会离你远去。我肯定能实现你对恋爱的期待，治愈你的‘疫后综合征’”");
            g("他并没有回答我什么，只是一直盯着古诗文小本看。我能肯定他还在怀念和Sunny在一起的日子。");
            g("我的强推战术失败了，可我还是不甘心。他居然对这么可爱的女孩子没有感觉。我有些失望地回了自己的座位。");
        case 2:
            lovePoint -= 5;
            g("我快速遛回了自己的座位，然后我趁他低头复习时偷偷回头看了看他。");
        }
        break;
    }
    w(lovePoint);

    // 02 - 第一课

    const string CHOICES_FOLLOW[] = {"跟着", "不跟着", ""};
    const string CHOICES_MEMORY[] = {"你都写过什么", "咱们一起回班吧", ""};
    bool memory = false; // 是否听Richard讲了关于表白墙的回忆

    g("第一节课被统一的广播占了，我感觉自己的心情明显不像网课时那么低落、无力。看着这熟悉又陌生的教室，我自然地回想起了初一时的事情。");
    g("根据和Richard关系比较好的一个男生讲的，他在两年前的这个时候就已经靠近Sunny。那时我还完全在自己的小圈子里。");
    g("直到18年的428春季运动会。那一天，他拿着一个神秘的“八班加油”，还不让大家看夹在里面的内容。他的哥们儿还对我说过：“那天早上，Sunny莫名其妙地坐在了他身边。”");
    g("那天下午，他的事被几个女生揭开了。我的疑惑也解开了。自此之后，他开始疯狂地追求Sunny，但一直无果，持续了两三个月，然后他开始玩“暗示流”。每一次，他的言语都能逗笑我。");
    g("关注“Sunny事件”后，我开始对他有了一丝兴趣。到了初二以后，他的学习成绩开始猛涨，甚至进了年级前十；而他变得更加内向了，只和固定的两三个男生说话。");
    g("他这种怪异的性格更吸引了我的注意，但那时他似乎总是回忆着旧情而且排斥异性。我怕自己被他拒绝，就没能鼓起勇气与他交往。");
    g("广播结束，下课了。他走出了教室的后门。我好奇地跟了出去。他在走廊里徘徊了一圈，然后进入了我们班曾经于此上课的小教室。");
    p("\x11\x0f* (要跟着他吗？)");
    switch (choices(CHOICES_FOLLOW)) {
    case 0:
        lovePoint += loveBonus * 5;
        g("我走进了那个小教室，悄悄地小步靠近他。他正面对着一片画满涂鸦的墙，盯着看了一两分钟，然后把它拍了下来。");
        g("我轻轻地问他：“Richard，你在看什么呢？”");
        if (loveRush) {
            g("他似乎被吓到了，立刻回了头，回答道：“墙上的字。”然后又入了迷地看了起来。");
            g("他简短而严肃的回答让我心里一冷。刚才的表白起了巨大的‘负’作用，加剧了他对女生的排斥。");
            g("他继续死盯着墙上的涂鸦，完全不想和我多说话。过了两三分钟，他转身就走，回了班。");
            g("我看他走了，我也跟了出去。他回到了自己的座位，开始写起了物理卷子。");
        }
        else {
            g("他慢慢地转过身来，看着我说：“这里有我两年前留下的痕迹。看到这些东西，我就会想起那时候的事情，心里会畅快些。”");
            g("我把与涂鸦墙的回忆分享给了他：“我还记得，七年级时咱们班里的柱子上全是字，被称作表白墙。我还在上面写过我的偶像呢。”");
            p("\x11\x0f* (要对他说什么？)");
            switch (choices(CHOICES_MEMORY)) {
            case 0:
                lovePoint += loveBonus * 20;
                memory = true;
                g("我又问了一下他：“你都写过什么？说说你的回忆，我不会告诉别人。”");
                g("“初一下学期，大概是两年前的稍早些时候，我在八班的表白墙上写过一个动漫人物的名字。我还记得原文字是‘表白Nadeko’。”");
                g("我想知道他的更多往事，于是我柔柔地请求他：“我喜欢听你讲故事，哪怕虚构的也行。”");
                g("“哎，这还是初一上学期的事。那个时候，大概是九月中上旬，我听到班里有人放清甜女声歌曲，还跟着舞蹈。《恋爱循环》这个歌名我后来才知道。”");
                g("“刚听到时，我还把它的前奏听成了Side by Side视频的片头旋律。运动会上，初一的女生跳了这个舞蹈，其中有我们班的几个小女生。”");
                g("“自此之后，我便没再听到这首歌。可是它的欢快旋律却挥之不去。期末考试那天的中午，我在五考场，出来时发现了三考场的异常动静。我这过去一看，便走上了一条‘不归路’。”");
                g("“有许多人围着看大屏幕上的鬼畜(恶搞)视频。我一下子听出了熟悉的旋律，再一看，这视频名叫‘8848太监循环’。我在这里又反复看了几遍才走开。”");
                g("“初一寒假，由于我的成绩不是很理想，又黑暗、又孤独、又颓废。我几乎每天都会听这些歌，无论时间、地点、我的心情。”");
                g("“初一下学期，我看了它所出自的动漫《化物语》。可能是因为每天都看，我对这歌曲中的女主有了一种特殊的喜欢，类似你们所说的追星或是偶像。”");
                g("我一边听着，一边和他一起回忆着。也许，这就是一个内向男生的真实内心。身有万千情感回忆却不主动外露，只等一个有缘人来一同追忆回味。");
                g("还有三分钟就上课了。他对我说：“我与《恋爱循环》这首歌的故事还有很多，它对我的影响至关重大，先讲这么多吧。”然后转头大步走出小教室。");
                g("我并排地走在他身边，回了班。他回到了自己的座位，开始写起了物理卷子。");
                break;
            case 1:
                g("“咱们一起回班吧，我想看看你怎么做题。”");
                g("他又呆呆地看了一会儿，然后转头大步走出小教室。我并排地走在他身边，回了自己班。他回到了自己的座位，开始写起了物理卷子。");
                g("我在他身边看了几分钟。虽然我没有看明白他的思路，但他认真的样子确实让我着迷。");
                break;
            }
        }
        break;
    case 1:
        g("我没有继续尾随他，返回了自己的座位。我看着重逢的同学们欢快如故，仿佛这疫情从未发生过一样。");
        g("我等了许久，两分钟铃时，他才回来。");
        break;
    }
    w(lovePoint);

    // 03 - 心理工作

    g("第四节课下课后，化学老师等了两分钟才放我们下去吃饭。Richard立刻就冲了出去。");
    g("我顶着大风，走出了教学楼，却路遇年级组长设的分流卡口，堵了几分钟。历尽千辛万苦的防疫关，我终于来到了食堂二楼。");
    g("当我打完饭，去寻找自己的座位时，Richard已经吃完了。在被老师“重兵把守”的食堂，我只能眼睁睁地看着他走远，不敢上前拉住他。");
    g("开学第一天，我们如期刷了卷子讲了问题，但也意外连连。我所在的8B班却完美地避开了。");
    g("第五节课时，楼道里传来大量脚步声。楼道西端的7B班突然紧急转移。下课后，听政治老师说的：他们的测温枪不准，一高一低，保险起见才转移至备用教室。");
    g("紧接着，外面响起了做操的音乐，却没有老师引导我们下楼。8B班中做什么的都有。甚至有两人无视防疫要求，近距离对视了一会儿");
    g("他们俩的深情又一次激起了我对Richard的幻想：等到考试结束后或毕业后，我一定要和他独处一段时间，近距离的了解他。");
    g("晚上，我写完了作业，看了看他的消息。他在朋友圈里发了小教室涂鸦墙和校园其他地方的照片，配着文字“览物之情，得无异乎”。");
    g("结合他初一时的经历和涂鸦墙很久以前就有的事实。“得无异乎”应该是指他在初一和现在看到墙上涂鸦时心情迥然不同。");
    if (memory) {
        g("他和我讲初一的往事时，他的语气完全变了。他一定很怀念初一的生活，才会在讲述时那么走心，那么愿意和我分享，和我共同追忆。");
        g("我的脑子中冒出了一个大胆的想法：我要尽量让他进入回忆的状态，这样我可以趁机卸下他防卫的外衣，治愈一直被他埋在心底的旧伤。");
        g("这个计划也有一点小问题。就是我没有了解过他的内心，不知道哪里是他最留恋的地方。");
    }
    w(lovePoint);

    // 04 - 《神奇的水滴石》

    const string CHOICES_PHONE[] = {"赶紧复习", "你在看什么呢", "不去打扰他", ""};

    g("2020.4.23。我进班时，已经有一半的同学来了。Richard居然没有像前几天一样写古诗文小本，他在看手机。");
    p("\x11\x0f* (要对他说什么？)");
    switch (choices(CHOICES_PHONE)) {
    case 0:
        lovePoint += loveBonus * 3;
        g("我向他走过去，对他说：“Richard，赶紧复习!”他的手机上显示的是密密麻麻的文字。");
        g("他立刻收起了藏在课桌下的手机，开始写起卷子。他在写的时候，我回头看了看他的身姿。");
        g("晚上八点多，Richard的好哥们儿Roy给我发了一个东西。他说这是Richard让他发的。");
        g("发来的文件是Richard的小说\x11\x0b《神奇的水滴石》\x12，还有Richard附上的一句话“你要是能读懂它的深层含义，明天可以来和我说一说。”");
        g("刚开始读时，我觉得他写的有些难以理解，主角们的行为又似曾相识；但是，读到后半部分时，我却把自己读了进去。");
        g("故事中，伊楠斯的性格和现实中的Richard几乎一模一样。茵苏因车祸而死对伊楠斯的沉重打击应该对应Sunny与他越走越远。而我的角色正是“长得和茵苏很像的梅古里”。");
        g("在带入到故事中想一想，Richard应该已经期待一个长得像Sunny的人很久了，说不定他还暗中欣赏过我呢。");
        g("我又仔细地品读了一下梅伊两人的第一次亲密。梅古里对他说：“让我来治愈你心底的旧伤。”他把梅古里看做曾经的茵苏，要把曾经的热情全部给她。");
        g("再一想，只要我表现的足够像Sunny，Richard应该就会喜欢我。");
        g("我又读了一会儿，那种喜爱的感觉已经快要爆发。Richard仿佛就在我身边，和我讲着遥远的回忆。");
        break;
    case 1:
        lovePoint += loveBonus * 10;
        g("我向他走过去，对他说：“你在看什么呢？能给我看看么？”他的手机上显示的是密密麻麻的文字。");
        g("“这是我准备在结课考后发布的小说。你要想看的话，今天放学后提前发给你。”说着，他的表情立刻不那么严肃了，他似乎已经不那么排斥我了。");
        g("“你要是能读懂它的深层含义，明天可以来和我说一说。之前还没有像你这么主动的读者呢。”他补充道。");
        g("下午回家后，我就开始看着手机焦急等待。到了晚上八点多，Richard才把他的小说\x11\x0b《神奇的水滴石》\x12发给了我。");
        g("刚开始读时，我觉得他写的有些难以理解，主角们的行为又似曾相识；但是，读到后半部分时，我却把自己读了进去。");
        g("故事中，伊楠斯的性格和现实中的Richard几乎一模一样。茵苏因车祸而死对伊楠斯的沉重打击应该对应Sunny与他越走越远。而我的角色正是“长得和茵苏很像的梅古里”。");
        g("在带入到故事中想一想，Richard应该已经期待一个长得像Sunny的人很久了，说不定他还暗中欣赏过我呢。");
        g("我又仔细地品读了一下梅伊两人的第一次亲密。梅古里对他说：“让我来治愈你心底的旧伤。”他把梅古里看做曾经的茵苏，要把曾经的热情全部给她。");
        g("再一想，只要我表现的足够像Sunny，Richard应该就会喜欢我。");
        g("我又读了一会儿，那种喜爱的感觉已经快要爆发。Richard仿佛就在我身边，和我讲着遥远的回忆。");
        break;
    case 2:
        g("我不想打扰他，就在自己的座位上回头看了一会他。他的手机上显示的是密密麻麻的文字。");
        g("晚上八点多，Richard的好哥们儿Roy给我发了一个东西。他说这是Richard让他发的。");
        g("发来的文件是Richard的小说\x11\x0b《神奇的水滴石》\x12，还有Richard附上的一句话“你要是能读懂它的深层含义，明天可以来和我说一说。”");
        g("刚开始读时，我觉得他写的有些难以理解，主角们的行为又似曾相识；但是，读到后半部分时，我却把自己读了进去。");
        g("故事中，伊楠斯的性格和现实中的Richard几乎一模一样。茵苏因车祸而死对伊楠斯的沉重打击应该对应Sunny与他越走越远。而我的角色正是“长得和茵苏很像的梅古里”。");
        g("在带入到故事中想一想，Richard应该已经期待一个长得像Sunny的人很久了，说不定他还暗中欣赏过我呢。");
        g("我又仔细地品读了一下梅伊两人的第一次亲密。梅古里对他说：“让我来治愈你心底的旧伤。”他把梅古里看做曾经的茵苏，要把曾经的热情全部给她。");
        g("再一想，只要我表现的足够像Sunny，Richard应该就会喜欢我。");
        g("我又读了一会儿，那种喜爱的感觉已经快要爆发。Richard仿佛就在我身边，和我讲着遥远的回忆。");
        break;
    }
    w(lovePoint);

    // 05 - 那节外教课

    const string CHOICES_THAT_CLASSROOM_1[] = {"你在看什么呢", "咱俩一起走吧", ""};
    const string CHOICES_THAT_CLASSROOM_2[] = {"为什么在这里上课", "那一天发生了什么", ""};
    const string CHOICES_AFTER_SCHOOL[] = {"跟着他", "出门右拐", ""};

    g("2020.4.24。倒春寒已经退去两天了。今天的午后，外面已经很温暖了。");
    g("然而，我为了吸引Richard的目光，早上就脱下了外套，完全露出里面的短袖校服。这样被凉到也值了。");
    g("今天下午，Richard在班里消毒擦地。我去楼下办公室找了老师一趟，遗憾地错过了帮他的机会。老师给我讲完后，我立刻从西侧楼梯跑上了四楼。");
    g("我急急忙忙地跑进了班，发现他已经不在了。迅速地背起书包，我立刻出去找他。他居然在初中楼四楼最东侧的备用教室旁呆呆的站着。");
    p("\x11\x0f* (要对他说什么？)");
    switch (choices(CHOICES_THAT_CLASSROOM_1)) {
    case 0:
        lovePoint += loveBonus * 5;
        g("我走了过去，问他：“Richard，你在看什么呢？”");
        g("“我们曾经上过课的教室。”他长舒了一口气，然后回答了我。");
        g("我有些记不清了，所以又问了他一下：“什么时候在这里上过课？”");
        p("“就是两年前的今天，那是一个周二的中午，第五节的外教课就是在这里上的。4月17日也有一节外教课在这里上。”");
        switch (choices(CHOICES_THAT_CLASSROOM_2)) {
        case 0:
            g("我用尽可能温柔的语气问他：“为什么在这里上课？”");
            g("他回答我：“2018年4月中上旬，8A班的英语周老师离开，8A8B只能合班上英语课。那时，恰逢8班教室的大屏幕坏了。”");
            g("“由于外教老师的课件需要大屏幕，所以我们全班在面前的这个教室一起上了两节外教课。那天进行了一个小辩论。”");
            if (lovePoint >= 45)
                g("“我还模糊地记得论题是早恋对学习有好处还是坏处。”");
            g("说罢，他拉起了他的拉杆包，缓缓走向楼梯间，下楼。");
            break;
        case 1:
            lovePoint += loveBonus * 15;
            g("我用尽可能温柔的语气请求他：“那一天你在这里都干了什么，能和我说说吗？”");
            g("他没有直接回答，而是又问了问我：“你要是把这些告诉别人怎么办？你为什么想问我这个？”");
            g("我想起了昨天晚上读过的他的小说，想着模仿一下试试。于是我凑了过去，小声地对他说：“我想当你的‘梅古里’，真心倾听你的心事，治愈你的旧伤。”");
            g("他睁大了半闭的双眼，看了我的脸几秒，然后开始讲起：“那天中午，我像一周前一样拿着上课的东西来到这间教室。”");
            g("“我来到这时，这里本有的座位已满，所以我又去对面的小教室搬了一把椅子。拿椅子回来后，我看见Sunny坐在最后一排靠中间的位置，就临时起意，坐在了她的右边。”");
            g("那时，我欣喜而有些心虚。我不敢和她说话，只敢时不时向左看看。我还清清楚楚的记得，她那时没穿外套。所以我看见了她右手手腕上的标志性黑点。");
            g("“为了不挤到她，我紧缩自己的身躯。没有乱动，甚至把腿偏向右侧。虽然她没有理我，但那时我懵懂的甜蜜感一点也没减。”");
            g("他讲完一段后，又问了问我：“现在你有没有想起来你在这里做过什么？”");
            g("或许是因为初一时我对校园生活投入不深，现在我对这段时间的具体事件已完全没有印象。“完全没有。我只知道我在这里上过课。”");
            if (lovePoint >= 70)
                g("他对我说：“有点晚了，家长已经等了一会了。我先回家了，明天再陪你。”说罢，他拉起了他的拉杆包，缓缓走向楼梯间，下楼。");
            else
                g("他对我说：“有点晚了，家长已经等了一会了。我先回家了。”说罢，他拉起了他的拉杆包，缓缓走向楼梯间，下楼。");
            break;
        }
        break;
    case 1:
        g("我走了过去，对他说：“Richard，咱俩一起走吧。”");
        g("他拉起了他的拉杆包，缓缓走向楼梯间，下楼。");
        break;
    }
    p("");
    g("我不声不响地跟在他身边，从初中楼走出来，左转，向大门走去。他在走的时候总是向左右张望，而且速度比往常慢。");
    g("离大门还有约二十多米时，他突然回头看向我，用温厚的嗓音和我道别：“"+name+"再见！”");
    g("“再见!”");
    p("走出大门后，他左转了，向着地铁站的方向走去。而我的家长则在大门右侧等我。");
    switch (choices(CHOICES_AFTER_SCHOOL)) {
    case 0:
        lovePoint += loveBonus * 20;
        g("出了大门后，我向左转并急走了几步，赶上了Richard。我走到了他的右前方。");
        g("他看见了我，向我靠近了一下，扭头看着我，惊喜地对我说：“你是不是和我有心灵感应？每当我追忆过往时就在我身边出现。”");
        g("我俏皮的回答他：“当然啦!没有人比我更懂Richard的心思。快说，你是不是在这里和Sunny做了些什么？”");
        g("他一边慢慢地走着，一边和我说着：“是的，而且正好是在两年前的今天，更巧的是和今天一样也是一起放学时。”");
        g("“那天放学后，我在班里等了等Sunny。她出来时，我就跟在她的侧后方。和你一样也在大门前互道了再见。”");
        g("“出了校门后，她居然也往这边走。我还记得，她当时走的位置比你靠外，在人行道下边。我一边走向公交站，一边看着可爱的她，就是没敢与她多聊聊。”");
        g("我听出了他的一丝叹惋，安慰了他：“多和我聊啦，释放一下你心中的压抑。你看看我，长得多像Sunny，可爱吧。”");
        g("走着走着，他停在了小路口前，并继续对我讲：“她在这里过了马路，然后继续向前走了一小段儿，乘她家长的车回家。我目送着她离开，还记下了她的车牌尾号9116。”");
        g("他看着车流源头的方向，也就是Sunny曾走的地方，说到：“在那趟803上，我开始幻想了更多美好的共同生活，想与她埋下一颗爱的种子，等到以后开花结果。哎——现在她可能注定要离开。”");
        g("他回头注视着我，和我告别：“这次我真的要回家了，你也早点回家吧。”虽然我们都戴了口罩，但我仍感觉到他的脸上再一次露出了幸福的微笑。");
        g("他上车后，我又远远地遥望了他乘的车。又走了一会，我找到了我妈的车。在回家的路途上，我情不自禁地幻想着Richard也在车上，他伸出手臂紧紧地搂着我。");
        break;
    case 1:
        g("出了大门后，我向右转，行数十米，找到了我妈的车。在回家的路途上，我情不自禁地幻想着Richard也在车上，他伸出手臂紧紧地搂着我。");
        break;
    }
    w(lovePoint);

    // 06 - 四·二八

    const string CHOICES_GO_TO_PLACE[] = {"报国亭", "体育馆", "他刚才一直凝视的地方...？", ""};
    const string CHOICES_PAVILION[] = {"坐到他的身边", "坐到他的腿上", ""};
    const string CHOICES_REMOTE[] = {"坐到他的右边", "坐到他的腿上", "坐到他的左边", ""};

    g("2020.4.28。明天就考试了，可我还是有点心虚。这几天，我虽然问了Richard许多难题，但没基本听懂他的方法，只享受了和喜欢的人在一起的感觉。");
    g("今天早上，班里又出现了反常的现象：大家聊天的声音明显比前几天高。我去打听了一下，他们得到的官方消息：今年体育中考中的长跑因疫情取消了。");
    g("明天就考试了，而且长跑被取消了。但是今天上午体育课上老师还是让我们跑了两圈，做俯卧撑，蛙跳。这就像数学方程中的设而不求法一样：练而不考。");
    g("好在，练完无聊的项目后，老师给了我们自由活动的时间，允许我们去体育馆，但在里面要戴好口罩。");
    g("Richard站在跑道起跑线的旁边，一动不动的站着，面向操场拦网后的一扇紧锁的小门。他的奇怪行为又一次发作了。");
    g("我准备利用自由活动的时间，给他一个惊喜：我与他的幽会。我向他走了过去，邀请他：“Hey，Richard！我想带你去校园里走一走，怎么样？”");
    g("“好的，我现在没有什么事。你想去哪就去哪吧。”他侧过脸来，用余光看着我。我想一下了，觉得有三处地点他会喜欢。");
    p("\x11\x0f* (要去哪里？)");
    switch (choices(CHOICES_GO_TO_PLACE)) {
    case 0:
        lovePoint += loveBonus * 20;
        g("我向操场出口走去，准备前往我最喜欢停留的地方——报国亭。他则跟在我后边，一声不吭。我不经意间的一回头，发现他还在看着操场拦网与高中楼中间的那片空地。");
        g("我们来到报国亭上，我坐在了背对初中楼的一边。他则在我的对面缓缓地坐下，但他并没有看着我，而且没主动说话。");
        p("\x11\x0f* (趁四周无人，起身...)");
        switch (choices(CHOICES_PAVILION)) {
        case 0:
            g("我向四周环顾了一遍，四周无人，就立刻起身，坐到了他的身边。他没有对我做什么亲密的小动作，也没有反抗。");
            break;
        case 1:
            lovePoint += loveBonus * 10;
            g("我向四周环顾了一遍，四周无人。我就立刻起身，坐到了他的大腿上。他没有对我有什么亲密的小动作，也没有反抗。");
            g("他的大腿很有弹性，和我的身体相当贴合。我坐在上面比直接坐在报国亭的座子上舒服多了。");
            if (lovePoint >= 80 && loveBonus > 1)
                g("“真的好像Sunny坐在了我身上啊。身材娇小、又轻又软，"+name+"挺可爱的。”他在我耳边小声的说。");
            break;
        }
        g("“以前放学时，我经常来这里坐。看看晚霞，听听我喜欢的歌，或者盯着放学回家的同学们。Richard，你喜欢在这里做什么？”");
        g("他回答我：“我不经常来这儿。但我还清楚地记得，两年前的早些时候，我在下楼上体育课时路过了这里。”");
        if (amanda)
            g("“那一天，我随身携带了一把钢尺，在这用它弹了一小段《恋爱循环》。在我旁边听的有几个女生，你应该也在其列。自此之后，我很少来这里。”");
        else
            g("“那一天，我随身携带了一把钢尺，在这用它弹了一小段《恋爱循环》。在我旁边听的有几个女生，其中一个应该是Amanda。自此之后，我很少来这里。”");
       g("我趁机给他一个邀约：“以后，我想多带你来这里。下次，我会送你一把钢尺，听听你亲手弹的曲子。”");
        g("他答应了：“行。我还想了解一下你是怎么喜欢上我的。你为什么现在才开始追我？”");
        g("“我可以给你说说，但你一会也要告诉我你为什么喜欢Sunny，而且被拒后一直不放弃。”");
        g("“刚入学时，我并没有关注过你。两年前“Sunny事件”发生后，我开始对你有了一丝兴趣。到了初二以后，你的学习成绩开始猛涨，甚至进了年级前十，每次考试你的理科成绩都数一数二。”");
        g("“我对你的理科很崇拜，想让你亲自教我。你对她这种专一的感情更吸引了我的注意。但那时你变得更加内向了，而且排斥异性。我怕被你拒绝，就没能鼓起勇气与你交往。”");
        g("“在漫长而无聊的网课期间，我总是想起你，但还是不敢主动联系。我做了艰难的思想斗争，决定利用初三的最后时间去和你交往。无论你喜不喜欢我，我行动了，就不会后悔。”");
        if (lovePoint >= 80 && loveBonus > 1) {
            lovePoint += loveBonus * 5;
            g("“哎，Sunny已经离我远去了。现在我应该更加珍惜来之不易的喜欢我的人，"+name+"。毕竟你长得有点像Sunny。”他轻轻地流露出他的心声。");
        }
        g("他似乎也动情了，向我倾诉道：“你的经历又让我想起了我最怀念的时候，从两年前的四月上旬到两年前的今天，也就是我开始喜欢Sunny的时候。”");
        g("“两年前，我在第一次月考中失利，父母疯狂地批评我，说我将没有未来。那时，我开始对校园产生依赖，早去晚归，不想在家里多待。”");
        g("“七年级下册的道德与法治讲了关于青春期的东西和男女交往。我就尝试了一下：在美术课上画东西并与大家交流，中午时用《恋爱循环》的曲调唱元素周期表，用钢尺弹奏《恋爱循环》”");
        g("“起初，我开放自己的行动不定向地面向班上的同学。到了四月后，我开始渐渐地喜欢Sunny。之后，我想给她讲题，想要抓住机会与她接近一些。有时还会无意识地想起她的美貌。”");
        g("“两年前四月下旬的一天晚上，我写完作业后出去散步。我走着走着就想起了她，甚至幻想起了和她牵手一起漫步。”");
        g("我想了解为什么他喜欢人的是Sunny而不是别人，又问了一下他：“Richard，你为什么没有喜欢上别人，而只喜欢Sunny一人？”");
        g("我问完这个问题时，他并没有立即回答。思考了近半分钟，他才缓缓地回答我。");
        if (lovePoint >= 75) {
            g("“她的样貌让我难以抗拒，瘦小、四肢纤细、梳着马尾辫，穿着与众不同的黑色袖口校服。那时，我经常偷偷看她，看着看着就逐渐有了好感。");
            g("我有些欣喜，居然问出了我喜欢的人的不为人知的小心思。");
        }
        else
            g("“这个…我…有点记不清具体原因了。应该与我的审美有关，她的样貌恰好与我的喜欢标准重合了。”");
        g("我拉着他的手，在偌大的校园里一起走了走。一路上，我都在和他聊考试策略与做题技巧。显然，他是一个久经考验的老考手。");
        g("快下课时，我为了不让人发现我们俩的一次小小“偷情”，提前跑回了操场上的集合地。过了不到一分钟，他也走了过来。");
        g("对于我来说，Sunny一直以来是我的潜在竞争对手。这些天，她一直没有搭理Richard。这应该算是个好消息。");
        break;
    case 1:
        lovePoint += loveBonus * 30;
        g("我看到在体育馆里的人不太多，就准备去体育馆里玩玩。他则跟在我后边，一声不吭。我不经意间的一回头，发现他还在看着操场拦网与高中楼中间的那片空地。");
        g("我与他走进了体育馆，他走到中心篮球场与西侧羽毛球场之间的狭长空隙，停下了脚步，注视着这条空隙的靠北一侧。我能感受到，这里也是他怀念的地方。");
        g("我朝他走过去，拉住了他的手，正视着他。他似乎明白了我的动作提示，向我说道：“两年前，我在这里与Sunny打过羽毛球。”");
        g("“那是两年前的4月13日，一个阴雨绵绵而湿冷的周五。大课间的操取消了，我在班里看见Sunny和Shirley一起下楼，就跟了下去。”");
        g("“我追随她来到了体育馆，我看见她们打羽毛球，就去拿了一个地上的球拍。我向她请求，她居然直接答应了和我一起打。那天，由于在体育馆里的学生太多，其他场地已满，我与她们只能在这里打羽毛球”");
        g("他指着他脚下的地板，继续向我讲着：“那时，我就站在这里；Sunny站在我的前面。发球时，羽毛球擦过她的发梢。当时，我还想把球发到她的头上，但总是差一点。她的小小背影对于我来说十分可爱。”");
        g("他又指向了体育馆北门的方向，说：“那边是Shirley的位置。比较高的球都被我接了。那一天的晚上，我还被父母带着去看了一场足球赛。”");
        g("我和他继续走着。今天体育馆里只有十几个人，远远比不上以前。走到体育馆的南门旁边时，他又停下了。");
        g("他对我说道：“两年前的一个周四，我做完卫生，来体育馆遛了一圈，发现Sunny和Shirley在这里打羽毛球。还有一次，她们在体育馆东南角的场地打。”");
        g("“还是那个周四，做卫生时，我听Sunny说Shirley很胖。我就用参照物的方式比较了我与她俩的相对胖瘦，用严谨的语言讲给她俩听。”");
        g("我和他一起坐在篮球框的底座上，我和他聊考试策略与做题技巧。显然，他是一个久经考验的老考手。");
        break;
    case 2:
        lovePoint += loveBonus * 30;
        g("我发现Richard刚才一直望着操场拦网与高中楼中间的一小片空地。");
        g("我猜，Richard一定是很怀念那里，才会一直看向那儿。他极有可能在那里邂逅过Sunny。所以，我准备带他去操场拦网与高中楼中间那片空地。");
        g("出了操场的铁网门，我向右转，走进这片空地。刚走出没几米，他就先问了我：“"+name+"，你还记得这里是干什么的地方么？”");
        g("我回答道：“运动会的观众席。去年十月的秋季运动会上，我去看你投实心球，还对你大喊‘加油’了呢。”他继续听着我的讲述，慢步往前走。");
        g("“运动会报名时，我听到你因胳膊有些疼不想参加，心里有些失望。那时我和Ryan一起劝你，开导你，但无济于事。最后也没报名。”");
        g("“运动会那天的中午，我听说Tony打篮球时受了些小伤，就想出了让你上场的计划。最后，在我与Tony、Ryan的联合劝说下，你上场了。”");
        g("“那时，你还很孤傲、内向，完全不愿多说一句话。我也就没再继续接触你。”");
        g("走着走着，他停在了那扇紧锁的小门前，然后他坐在了那门前的几级台阶上，向前面的空地上看着。");
        g("我回头看了看，其他男生多在操场上打球，还有一些人在练习中考项目，当然也有偷懒的。没有一个人在看着我们所在的‘偏僻’地方。");
        p("\x11\x0f* (要怎么做？)");
        switch (choices(CHOICES_REMOTE)) {
        case 0:
            lovePoint += loveBonus * 5;
            g("我贴着他宽大的身躯，坐在了他的右边。他看向我，说：“两年前的今天，Sunny就是像你这样主动坐在了我的左边。哎，那一次反常的接近却成了离别的开始。”");
            break;
        case 1:
            lovePoint += loveBonus * 10;
            g("我又向四周环顾了一遍，四周无人。我就立刻起身，坐到了他的大腿上。他没有对我做什么亲密的小动作，也没有反抗。");
            g("他的大腿很有弹性，和我的身体相当贴合。我坐在上面比直接坐在石质台阶上舒服多了。我转过头，渴求他说：“我想让你抱一下我。”");
            if (lovePoint >= 90 && loveBonus > 1)
                g("他并没有抱我“真的好像Sunny坐在了我身上啊。身材娇小、又轻又软，"+name+"挺可爱的。两年前的今天，我曾想让Sunny坐在我的腿上，她却没来。”他在我耳边小声的说。");
            else
                g("他并没有抱我，有些伤感地说：“嗬——两年前的今天，我曾想让Sunny坐在我的腿上，她却没来。等了这么久，却只等来了一个和我交集不多的"+name+"。”");
            break;
        case 2:
            g("我贴着他宽大的身躯，坐在了他的左边。他对我说道：“两年前的今天，春季运动会，我们的座位就在前面的那片空地上。”");
            break;
        }
        g("他继续讲着他的回忆：“那天早上，我五点多就醒了。我用蜡笔给昨晚的半成品写上‘Sunny加油’，并把它藏在了已做好的‘八班加油’之下，拍照发在了微信群里。”");
        g("“到校后，我在校门口买了些零食饮料，然后去班里吃了早点。我准备下楼时，Ryan进班了，我和他聊了一会，然后搬着椅子下楼。”");
        if (lovePoint >= 90)
            g("“下楼前，我让他猜了一下我的女朋友是谁。我给他了一个提示‘体重不到我的三分之一。’他就猜出来了。我赶紧说了一句‘不是。’”");
        g("“起初，我坐在第一排从左往右数第一个。但很快，写加油稿的把我赶走，我来到了第二排。不知等了多久，Sunny来了，她莫名其妙地坐在了我的右边。”");
        g("“那时，我认为她坐在我身边就是在向我发暗号。所以，我在上午的初一女子400米时就去给她加油举牌，位置就在终点线边上。”");
        g("“在那个明媚的上午，我拿着牌子，跟在她后面来到体育馆门口的检录处。然后我高举牌子，在起跑线旁边呐喊‘八班加油’。发令枪响后，她瞬间冲了出去，我留在原地远望着操场另一头的她。”");
        g("“当她快进入最后一个直道时，我高举并挥舞手中的‘八班加油’；她最后冲刺时，我抽走了上面的‘八班加油’，露出了底下的‘Sunny加油’。”");
        g("“比赛结束后，她没来和我说话。我回了自己的座位，她基本没再坐在我身旁。我记得，她的成绩并不太理想。”");
        g("“中午，我去体育馆找她，她在和Shirley打羽毛球。这次我有些害羞了，没敢过去和她一起打。天气有些热了，我拿两张牌子当扇子用。”");
        g("“下午的初一女子800米，我再次入场给她加油，流程和上午差不多，她却再一次冷落了我。但是，我回到观众席时，却被班上的几个女生叫住了。”");
        g("“她们反复问我‘你是不是喜欢Sunny？’之类的问题。我否认了几次，她们不让，最后我承认了我喜欢Sunny。”");
        g("“放学时，Sunny也没有理我。此后，她开始与我越走越远，但我一直喜欢着她。哎，我真的没想到，我最期待的一次运动会竟成了漫长而曲折的离别的开端。”");
        g("“自从那时起，我对其他女生不再有兴趣。即使我觉得某个女生比她还可爱，我也只是看两眼，然后不吭声地走开。我对其他人，尤其是女生，的防备之心增加了不少。”");
        g("我赶紧开导他：“有些人注定要走，想留都留不住；有些人注定要来，想躲都躲不了。好好珍惜当下的人和事，不要留下遗憾。”");
        g("他只是小声地耳语了一句：“好的，我会试着改变自己。”");
        break;
    }
    p("");
    g("下课后，我好奇地问了问他：“Richard，你为什么这么喜欢和我聊过去的回忆呢？”");
    g("他看了看遥远的地方，然后看着我回答：“那些是我最怀念的事，承载着我曾经拥有的短暂的幸福与甜蜜。在大疫临头的灰暗生活中，这些美好的回忆能稍微稳定我的情绪，使我晚些失去理智。”");
    g("“你在我身边时，回忆有了重现的载体，变得更加清晰了。我能与自己的回忆更加亲密地接触，所以我的心情才会变好一些”");
    g("我凑了过去，娇娇地对他说：“那你就把你的回忆代入我，怎么样？你一定想抱Sunny吧!来，让我抱抱。”");
    g("他赶紧往后退了几米，有些被吓住了：“回忆并不需要真实的重演，那样反而会毁掉那种只能幻想不能行动的距离感与期待感。虽然这里没人会看见。”");
    g("我看他是被疫情折磨坏了脑子。竟失去了爱与被爱的能力，时刻躲着与异性的亲密接触。");
    g("他小声而有些悲伤的告诉我：“我只是害怕这段恋情会像两年前一样再次消逝。那样既会让你体会失恋的痛苦，你的痛苦也会给我带来负罪感。所以我一直以来才会这样。”");
    g("我赶紧安慰他：“我会一直守护在你的身边，无论发生了什么。”");
    g("他默默地走回了班，我跟在他身边一起回去了。他低着头，在思考着......");
    g("放学后，他主动地来找我，说：“我想在考前最后一晚再认真复习一次，你能不要来打扰我么。”");
    g("我深情地望着他，回答：“好的，那我先不问你了。你要是心中有什么事过不去，就来和我聊一聊。我就是你的‘梅古里’。”");
    if (lovePoint >= 70)
        g("他临走时来给我加了加油：“你可以的，"+name+"。祝你结课考顺利!”");
    w(lovePoint);

    // 07 - 结局

    g("两天的结课考，在心神十分紧张的状态下，一转眼就过去了。2020.4.30。最后一场考试结束后，我从考场上到四楼，回到了七年八班的教室。Richard好像又看了一下Sunny。");
    g("老师给我们进行了疫情防控教育、安全教育，然后收上了个人健康卡。历史老师还拿来了两套卷子作为五一假期的作业。");
    g("老师说可以放学回家后，我立即去找了Richard，对他说：“Richard，去报国亭等一下我，我做完卫生就下来找你，给你一个惊喜。”");
    g("由于五一假期有领导来检查卫生，所以班主任老师让我们多打扫了一会。做完卫生后，我立刻拿书包下了楼，走向报国亭。现在，已经放学约半个小时了，学校里的学生大部分都回家了。");
    if (lovePoint >= 130) { // 真爱结局
        g("Richard在报国亭上看着我。我快走到他身前时，他站了起来。我看周围没人，就冲上去，使了很大的力气抱住他。我快把自己挤的窒息了，但他却没什么反应。");
        g("我放开了自己的双臂，然后拉着他坐了下来。他先问了我一句：“你选择保送还是选择中考，打算好了吗？”");
        g("我高兴地回答他：“当然要和你一起了。我最喜欢你了。我猜，你肯定想要保送。因为我感觉到你身上的备考紧迫现象已经完全消失。”");
        g("他看着我，向我说：“是的。我很喜欢小外，也很喜欢这里的生活。现在，疫情给我们的生活带来这么大的影响和威胁，我可不想考到陌生的学校。我也承受不起近三个月的漫长备考。”");
        g("我见他只字不提我和他的关系，就把手臂搭在他的肩上，笑着问了他：“你喜欢的不只是小外吧。快说出来吧，让我听听。”");
        g("他瞒不住了，向我吐露了他的真心：“其实，我的真爱一直是Sunny，尽管她已远去。两年来，我一直没有中断过思念与期待。”");
        g("“只不过，这一周多你一直陪着我，倾听我的心事，一起回忆美好的过去。我不高兴时，你就来安慰我，开导我。让我有了一个轻松的心情来参加最后的决战——初三结课考。”");
        g("“以后我会试着像爱Sunny一样爱你，因为你和我理想中的情人一样，而且"+name+"你长得也有点像Sunny。我可能会有些放不开，你要教教我如何加深感情。”");
        g("我心中欣喜极了，他居然喜欢上我了。我邀请他：“这个五一假期，我想和你一起在市内徒步游览一下，行吗？”");
        g("他摸了摸我的头，答应了我的邀请。我拉着他的手，我们互相看着彼此。然后，我走在他身旁，和他一起走出了校园，就像六天前那样。");
        g("“再见，Richard!”");
        g("“再见，"+name+"!”");
        w(lovePoint);
        p("\n\x11\x0cTruelove End：第二个Sunny");
    }
    else if (lovePoint >= 90) { // 好结局
        g("Richard在报国亭上看着我。我快走到他身前时，他站了起来。我看周围没人，就冲上去，使了很大的力气抱住他。我快把自己挤的窒息了，但他却没什么反应。");
        g("我放开了自己的双臂，然后拉着他坐了下来。他先问了我一句：“你选择保送还是选择中考，打算好了吗？”");
        g("我高兴地回答他：“当然要和你一起了。我最喜欢你了。我猜，你肯定想要保送。因为我感觉到你身上的备考紧迫现象已经完全消失。”");
        g("他看着我，向我说：“是的。我很喜欢小外，也很喜欢这里的生活。现在，疫情给我们的生活带来这么大的影响和威胁，我可不想考到陌生的学校。我也承受不起近三个月的漫长备考。”");
        g("我见他只字不提我和他的关系，就把手臂搭在他的肩上，笑着问了他：“你喜欢的不只是小外吧。快说出来吧，让我听听。”");
        g("他瞒不住了，向我吐露了他的真心：“其实，我的真爱一直是Sunny，尽管她已远去。两年来，我一直没有中断过思念与期待。;”");
        g("“只不过，这一周多你经常陪着我，倾听我的心事，一起回忆美好的过去。我不高兴时，你就来安慰我，开导我。让我有了一个轻松的心情来参加最后的决战——初三结课考。”");
        g("我有些欣喜，他对我有好感了。我邀请他：“这个五一假期，我想和你一起在市内徒步游览一下，行吗？”");
        g("他痛快地答应了我的邀请。我拉着他的手，我们互相看着彼此。然后，我走在他身旁，和他一起走出了校园，就像六天前那样。");
        g("“再见，Richard!”");
        g("“再见，"+name+"!”");
        w(lovePoint);
        p("\n\x11\x0dGood End：知心恋人");
    }
    else if (lovePoint >= 50) { // 普通结局
        g("Richard坐在报国亭上，双臂放在腿上，低着头，似乎在思考着什么，并没有考试结束后该有的放松。我走上前去，并坐在了他身边。他先问了我一句：“你选择保送还是选择中考，打算好了吗？”");
        g("我高兴地回答他：“当然要和你一起了。我最喜欢你了。我猜，你肯定想要保送。因为我在初二时听说过你想保送到小外的高中部。”");
        g("他对我说：“是的。我很喜欢小外，也很喜欢这里的生活。现在，疫情给我们的生活带来这么大的影响和威胁，我可不想考到陌生的学校。我也承受不起近三个月的漫长备考。”");
        g("我见他只字不提我和他的关系，就把手臂搭在他的肩上，笑着问了他：“你喜欢的不只是小外吧。快说出来吧，让我听听。”");
        g("他瞒不住了，向我吐露了他的真心：“其实，我的真爱一直是Sunny，尽管她已远去。两年来，我一直没有中断过思念与期待。”");
        g("“哎，她已经不搭理我很久了。现在，我什么也不想做，只想等着保送的通知下来。如果Sunny保送了，那我会再与她交往一回，冰释前嫌；如果她没有，那我就在保送班试着去适应没有她的生活。”");
        g("我立刻开导他：“既然Sunny已经与你不再有瓜葛，那你为什么还不放弃？你可以一点点地试着把自己解脱出来，去面对新的生活与新的朋友。”");
        g("他有些低沉地回答了我“因为Sunny还没说她是否保送。在最终结果下来之前，还是有最后的希望。”");
        g("“还有，如果你真的那么喜欢我，你还可以继续来与我交往。我会试着接纳一个新的伴侣，无论我是否喜欢这个人。”");
        g("他虽然对我没多少好感，但也不排斥我了。这么一想，我的机会还很多，于是我邀请他：“这个五一假期，我想和你一起在市内徒步游览一下，行吗？”");
        g("他答应了我的邀请。然后，我走在他身旁，和他一起走出了校园，就像六天前那样。");
        g("“再见，Richard!”");
        g("“再见，"+name+"!”");
        w(lovePoint);
        p("\n\x11\x0bNormal End：新的伴侣");
    }
    else { // 坏结局
        g("远远看去，报国亭上一个人也没有。我的心一下就紧张了起来。我一边往报国亭走，一边向四处张望，可是周围只有一个不认识的老师经过。");
        g("我问了那位途经这里的老师，他说他工作很忙，也没有看见Richard。我最担心的事情还是发生了，Richard应该是趁我做卫生时逃走了。");
        g("我失望地坐在了寂寥的报国亭上，我期待了这么久，而他就这样离开了我。或许，这就是他的怪异性格。");
        g("我又联想起他追Sunny的经历，他也经历过这种被喜欢的人拒绝的失望感。或许，他现在的感受正和Sunny两年前的感受一样。");
        if (loveRush)
            g("都怪我太专注于自己的感觉与享受，却几乎没有倾听几回他的心事。我的贸然表白还让他产生了反感。");
        else
            g("都怪我太专注于自己的感觉与享受，却几乎没有倾听几回他的心事。");
        g("我在责怪自己时，发现了地上的一张纸。我好奇地捡起了它。把它展开铺平。它上面写着“我只想一个人过平静的生活。”字迹明显是Richard的。");
        g("我孤身一人走出了校门，上车，回了家。到家后，我又给他发了一次消息，想要把他挽留在我身边。");
        g("他回复了我：“一切都回不去了。”之后，整个五一假期，他都没与我聊天。");
        w(lovePoint);
        p("\x11\x08\nBad End：永远的回忆"); // “\x08Bad”在某些环境下无法通过编译，特把“\n”移到后面来隔开
    }

    // 结束菜单

    const string CHOICES_ENDMENU[] = {"结束", ""};
    choices(CHOICES_ENDMENU);
    goto restart;
}