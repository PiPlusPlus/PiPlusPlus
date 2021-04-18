// version: 0.0.6.16 alpha
/*
运行前请在同一目录下创建hello-world.ppp（没打错，就是ppp），内容如下：
WRT 00000000 48
WRT 00000001 65
WRT 00000002 6C
WRT 00000003 6C
WRT 00000004 6F
WRT 00000005 2C
WRT 00000006 20
WRT 00000007 77
WRT 00000008 6F
WRT 00000009 72
WRT 0000000A 6C
WRT 0000000B 64
WRT 0000000C 21
PRT 00000000
PRT 00000001
PRT 00000002
PRT 00000003
PRT 00000004
PRT 00000005
PRT 00000006
PRT 00000007
PRT 00000008
PRT 00000009
PRT 0000000A
PRT 0000000B
PRT 0000000C
END
程序运行后输入文件名（hello-world.ppp），然后回车。
按照预期应该输出“Hello, world!”，已经单独测试成功。
现在的问题是打不开文件（macos），windows没问题，linux没测试过
*/
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#if __cplusplus >= 201103L
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cwunsigned char>
#include <cwctype>
#endif

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif

#include <time.h>
#include <sys/time.h>

using namespace std;

const bool
CONST_CHECK_MEM_SIZE=1,
CONST_CHECK_GOTO_POINT=1,
CONST_PPP_TEST_VER=1;

const int
CONST_MEN_SIZE_MAX=67108864,
CONST_GOTO_POINT_MAX=67108864,
CONST_PPP_VER=16;

const string
CONST_PPP_VER_F="0.0.6.16 alpha";

map<int,unsigned char>mem;
map<int,int>goto_point;

struct cmdo{
    string fnc;
    vector<string>plst;
};
struct cmd{
    string fnc;
    vector<int>plst;
};
vector<cmdo>cmdtmp;
int cnttmp;
vector<cmd>cmdfnl;
int ptr;
vector<string>cmdstrs;

void reterr(string ctr,string tip){
    cout<<"["<<ctr<<"]"<<tip;
    exit(0);
}

void check_file_open_error(bool suc){
    if(!suc)reterr("freader","文件打开失败");
}
void check_mem_size(){
    if(CONST_CHECK_MEM_SIZE&&mem.size()>CONST_MEN_SIZE_MAX)reterr("prunner","动态内存溢出");
}
void check_mem_search_error(int p){
    if(!mem.count(p))reterr("prunner","动态寻址失败");
}
void check_para_align_error_f(bool suc){
    if(!suc)reterr("freader","参数对齐失败");
}
void check_para_align_error_c(bool suc){
    if(!suc)reterr("clmaker","参数对齐失败");
}
void check_para_align_error_p(bool suc){
    if(!suc)reterr("prunner","参数对齐失败");
}
void check_cmd_unknow(bool suc){
    if(!suc)reterr("clmaker","指令解释失败");
}
void check_ptr_error(){
    if(ptr<0||ptr>=(unsigned)cmdfnl.size())reterr("prunner","指令指针溢出");
}
void check_goto_point_repeat(int p){
    if(goto_point.count(p))reterr("clmaker","锚点命名重合");
}
void check_goto_point_error(int p){
    if(!goto_point.count(p))reterr("prunner","锚点定位失败");
}
void check_goto_point_size(){
    if(CONST_CHECK_GOTO_POINT&&goto_point.size()>CONST_GOTO_POINT_MAX)reterr("clmaker","锚点列表溢出");
}
void check_length_8(string str){
    if(str.length()!=8)reterr("clmaker","指针命名错误");
}
void check_length_2(string str){
    if(str.length()!=2)reterr("clmaker","单位长度错误");
}
void check_div_zero_d(bool suc){
    if(!suc)reterr("prunner","除法除数为零");
}
void check_div_zero_m(bool suc){
    if(!suc)reterr("prunner","取余除数为零");
}
void check_argc_gt_2(bool suc){
    if(!suc)reterr("freader","主体参数溢出");
}

void t(string code){
    cmdstrs.push_back(code);
}
void wrt(int p1,unsigned char p2){
    mem[p1]=p2;
    check_mem_size();
}
void add(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    mem[p1]=mem[p2]+mem[p3];
    check_mem_size();
}
void sub(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    mem[p1]=mem[p2]-mem[p3];
    check_mem_size();
}
void mul(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    mem[p1]=mem[p2]*mem[p3];
    check_mem_size();
}
void div(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    check_div_zero_d(mem[p3]);
    mem[p1]=mem[p2]/mem[p3];
    check_mem_size();
}
void mod(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    check_div_zero_m(mem[p3]);
    mem[p1]=mem[p2]%mem[p3];
    check_mem_size();
}
void _and(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    mem[p1]=mem[p2]&mem[p3];
    check_mem_size();
}
void _or(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    mem[p1]=mem[p2]|mem[p3];
    check_mem_size();
}
void _xor(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    mem[p1]=mem[p2]^mem[p3];
    check_mem_size();
}
void lmv(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    mem[p1]=mem[p2]<<mem[p3];
    check_mem_size();
}
void rmv(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    mem[p1]=mem[p2]>>mem[p3];
    check_mem_size();
}
void ipt(int p1){
    mem[p1]=getchar();
    check_mem_size();
}
void prt(int p1){
    check_mem_search_error(p1);
    putchar(mem[p1]);
}
void cls(){
    for(int i=0;i<24;i++)putchar('\n');
}
void gto(int p1,int&_ptr){
    check_goto_point_error(p1);
    _ptr=goto_point[p1];
}
void cmp(int p1,int p2,int p3){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    mem[p1]=mem[p2]<mem[p3];
    check_mem_size();
}
void rmm(int p1,int p2,int p3,int p4,int p5){
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    check_mem_search_error(p4);
    check_mem_search_error(p5);
    check_mem_search_error((unsigned)mem[p2]|((unsigned)mem[p3]<<8)|((unsigned)mem[p4]<<16)|((unsigned)mem[p5]<<24));
    mem[p1]=mem[(unsigned)mem[p2]|((unsigned)mem[p3]<<8)|((unsigned)mem[p4]<<16)|((unsigned)mem[p5]<<24)];
    check_mem_size();
}
void wmm(int p1,int p2,int p3,int p4,int p5){
    check_mem_search_error(p1);
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    check_mem_search_error(p4);
    check_mem_search_error(p5);
    mem[(unsigned)mem[p1]|((unsigned)mem[p2]<<8)|((unsigned)mem[p3]<<16)|((unsigned)mem[p4]<<24)]=mem[p5];
    check_mem_size();
}
void cpy(int p1,int p2){
    check_mem_search_error(p2);
    mem[p1]=mem[p2];
    check_mem_size();
}
void gtw(int p1,int p2,int p3,int p4,int&_ptr){
    check_mem_search_error(p1);
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    check_mem_search_error(p4);
    check_goto_point_error((unsigned)mem[p1]|((unsigned)mem[p2]<<8)|((unsigned)mem[p3]<<16)|((unsigned)mem[p4]<<24));
    _ptr=goto_point[(unsigned)mem[p1]|((unsigned)mem[p2]<<8)|((unsigned)mem[p3]<<16)|((unsigned)mem[p4]<<24)];
}
void ifg(int p1,int p2,int&_ptr){
    check_mem_search_error(p1);
    check_goto_point_error(p2);
    if(mem[p1])_ptr++;
    else _ptr=goto_point[p2];
}
void del(int p1){
    check_mem_search_error(p1);
    mem.erase(p1);
}
void dlw(int p1,int p2,int p3,int p4){
    check_mem_search_error(p1);
    check_mem_search_error(p2);
    check_mem_search_error(p3);
    check_mem_search_error(p4);
    check_mem_search_error((unsigned)mem[p1]|((unsigned)mem[p2]<<8)|((unsigned)mem[p3]<<16)|((unsigned)mem[p4]<<24));
    mem.erase((unsigned)mem[p1]|((unsigned)mem[p2]<<8)|((unsigned)mem[p3]<<16)|((unsigned)mem[p4]<<24));
}
void end(int&_ptr){
    mem.clear();
    _ptr=-1;
}
/*
void rnd(int p1){
    mem[p1]=rand()&(255);
}
void srd(int p1,int p2,int p3,int p4){
    srand((unsigned)mem[p1]|((unsigned)mem[p2]<<8)|((unsigned)mem[p3]<<16)|((unsigned)mem[p4]<<24));
}
void tmr(int p1,int p2){
    timeval ttmp;
    gettimeofday(&ttmp, NULL);
    int tmp=ttmp.tv_usec%1000;
    mem[p1]=tmp&255;
    mem[p2]=tmp>>8;
}
void tmi(int p1,int p2){
    timeval ttmp;
    gettimeofday(&ttmp, NULL);
    int tmp=ttmp.tv_usec/1000%1000;
    mem[p1]=tmp&255;
    mem[p2]=tmp>>8;
}
void tms(int p1){
    time_t ttmp;
    time(&ttmp);
    struct tm *tmp;
    tmp=localtime(&ttmp);
    mem[p1]=tmp->tm_sec;
}
void tmm(int p1){
    time_t ttmp;
    time(&ttmp);
    struct tm *tmp;
    tmp=localtime(&ttmp);
    mem[p1]=tmp->tm_min;
}
void tmh(int p1){
    time_t ttmp;
    time(&ttmp);
    struct tm *tmp;
    tmp=localtime(&ttmp);
    mem[p1]=tmp->tm_hour;
}
void tmd(int p1){
    //日
}
void tmo(int p1){
    //月
}
void tmy(int p1){
    //年后二
}
void tmc(int p1){
    //年前二
}
void tmo(int p1,这里有一堆记得改){
    //时间
}
*/
unsigned char h2d2(string hex){
    unsigned char ret=0;
    for(int i=0;i<2;i++){
        ret<<=4;
        ret|='0'<=hex[i]&&hex[i]<='9'?hex[i]-'0':hex[i]+10-'A';
    }
    return ret;
}
int h2d8(string hex){
    int ret=0;
    for(int i=0;i<8;i++){
        ret<<=4;
        ret|='0'<=hex[i]&&hex[i]<='9'?hex[i]-'0':hex[i]+10-'A';
    }
    return ret;
}
void rdfl(string fn){
    char _fn[65536];
    for(int i=0;i<fn.length();i++)_fn[i]=fn[i];
    ifstream fin(_fn);
    // istream fin;
    // freopen("hello-world.ppp","r",fin);
    // stringstream fin;
    // for(int i=0;i<cmdstrs.size();i++)fin<<cmdstrs[i]<<"\n";
    string typ,tmp;
    // int lcnt;
    // fin>>lcnt;
    // lcnt=cmdstrs.size();
    // fin.open();
    if(fin.is_open()){
        while(!fin.eof()){
        // while(lcnt--){
            fin>>typ;
            if(typ=="WRT"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_2(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="ADD"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="SUB"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="MUL"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="DIV"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="MOD"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="AND"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="NOR"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="XOR"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="LMV"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="RMV"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="IPT"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="PRT"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="CLS"){
                cmdo ct;
                ct.fnc=typ;
                cmdtmp.push_back(ct);
            }else if(typ=="SPT"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="GTO"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="CMP"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="RMM"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="WMM"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="CPY"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="GTW"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="IFG"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="DEL"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="DLW"){
                cmdo ct;
                ct.fnc=typ;
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                fin>>tmp;check_length_8(tmp);ct.plst.push_back(tmp);
                cmdtmp.push_back(ct);
            }else if(typ=="END"){
                cmdo ct;
                ct.fnc=typ;
                cmdtmp.push_back(ct);
            }else{
                check_para_align_error_f(0);
            }
        }
    }else{
        check_file_open_error(0);
    }
}
void mkcl(){
    for(int i=0;i<cmdtmp.size();i++){
        if(cmdtmp[i].fnc=="WRT"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            unsigned char _p2=h2d2(cmdtmp[i].plst[1]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="ADD"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="SUB"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="MUL"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="DIV"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="MOD"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="AND"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="NOR"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="XOR"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="LMV"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="RMV"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="IPT"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="PRT"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="CLS"){
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="SPT"){
            check_goto_point_repeat(h2d8(cmdtmp[i].plst[0]));
            goto_point[h2d8(cmdtmp[i].plst[0])]=cmdfnl.size();
        }else if(cmdtmp[i].fnc=="GTO"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="CMP"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="RMM"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            int _p4=h2d8(cmdtmp[i].plst[3]);
            int _p5=h2d8(cmdtmp[i].plst[4]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            ct.plst.push_back(_p4);
            ct.plst.push_back(_p5);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="WMM"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            int _p4=h2d8(cmdtmp[i].plst[3]);
            int _p5=h2d8(cmdtmp[i].plst[4]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            ct.plst.push_back(_p4);
            ct.plst.push_back(_p5);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="CPY"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="GTW"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            int _p4=h2d8(cmdtmp[i].plst[3]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            ct.plst.push_back(_p4);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="IFG"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="DEL"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="DLW"){
            int _p1=h2d8(cmdtmp[i].plst[0]);
            int _p2=h2d8(cmdtmp[i].plst[1]);
            int _p3=h2d8(cmdtmp[i].plst[2]);
            int _p4=h2d8(cmdtmp[i].plst[3]);
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            ct.plst.push_back(_p1);
            ct.plst.push_back(_p2);
            ct.plst.push_back(_p3);
            ct.plst.push_back(_p4);
            cmdfnl.push_back(ct);
        }else if(cmdtmp[i].fnc=="END"){
            cmd ct;
            ct.fnc=cmdtmp[i].fnc;
            cmdfnl.push_back(ct);
        }else{
            check_para_align_error_c(0);
        }
    }
}
void do_prog(){
    ptr=0;
    do{
        check_ptr_error();
        if(cmdfnl[ptr].fnc=="WRT"){
            wrt(cmdfnl[ptr].plst[0],(unsigned char)cmdfnl[ptr].plst[1]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="ADD"){
            add(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="SUB"){
            sub(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="MUL"){
            mul(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="DIV"){
            div(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="MOD"){
            mod(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="AND"){
            _and(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="NOR"){
            _or(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="XOR"){
            _xor(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="LMV"){
            _xor(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="RMV"){
            _xor(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="IPT"){
            ipt(cmdfnl[ptr].plst[0]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="PRT"){
            prt(cmdfnl[ptr].plst[0]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="CLS"){
            cls();
            ptr++;
        }else if(cmdfnl[ptr].fnc=="GTO"){
            gto(cmdfnl[ptr].plst[0],ptr);
        }else if(cmdfnl[ptr].fnc=="CMP"){
            cmp(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="RMM"){
            rmm(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2],cmdfnl[ptr].plst[3],cmdfnl[ptr].plst[4]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="WMM"){
            wmm(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2],cmdfnl[ptr].plst[3],cmdfnl[ptr].plst[4]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="CPY"){
            cpy(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="GTW"){
            gtw(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2],cmdfnl[ptr].plst[3],ptr);
        }else if(cmdfnl[ptr].fnc=="IFG"){
            ifg(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],ptr);
        }else if(cmdfnl[ptr].fnc=="DEL"){
            del(cmdfnl[ptr].plst[0]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="DLW"){
            dlw(cmdfnl[ptr].plst[0],cmdfnl[ptr].plst[1],cmdfnl[ptr].plst[2],cmdfnl[ptr].plst[3]);
            ptr++;
        }else if(cmdfnl[ptr].fnc=="END"){
            end(ptr);
        }else{
            check_para_align_error_p(0);
        }
    }while(~ptr);
}
int main(int argc,char *argv[]){
    string fn;
    if(argc<=1){cout<<"Pi++ Interpreter (ver: "+CONST_PPP_VER_F+")\nRelative path of Pi++ program (End with <ENTER>): ";getline(cin,fn);}
    else if(argc==2)fn=argv[1];
    else check_argc_gt_2(0);
    rdfl(fn);
    mkcl();
    do_prog();
    return 0;
}
