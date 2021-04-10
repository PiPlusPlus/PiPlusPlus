# PiPlusPlus

**除特殊说明外，所有的参数（Pn）都为8位16进制数**

**所有的变量均为一个字节**

| 类型 | 格式与参数 | 具体描述 | 额外参数限制 |
|:-:|:-:|:-:|:-:|
| 写入常数 | `WRT P1 P2` | 将变量 `P1` 设置为常数 `P2` | `P2` 为2位16进制数 |
| 加法 | `ADD P1 P2 P3` | 计算变量 `P2` + 变量 `P3` 的值，并将结果写入变量 `P1` | 无 |
| 减法 | `SUB P1 P2 P3` | 计算变量 `P2` - 变量 `P3` 的值，并将结果写入变量 `P1` | 无 |
| 乘法 | `MUL P1 P2 P3` | 计算变量 `P2` * 变量 `P3` 的值，并将结果写入变量 `P1` | 无 |
| 除法 | `DIV P1 P2 P3` | 计算变量 `P2` / 变量 `P3` 的值，并将结果写入变量 `P1` | 变量 `P3` 不得为零，否则报错 |
| 取余 | `MOD P1 P2 P3` | 计算变量 `P2` % 变量 `P3` 的值，并将结果写入变量 `P1` | 变量 `P3` 不得为零，否则报错 |
| 按位与 | `AND P1 P2 P3` | 计算变量 `P2` & 变量 `P3` 的值，并将结果写入变量 `P1` | 无 |
| 按位或 | `NOR P1 P2 P3` | 计算变量 `P2` \| 变量 `P3` 的值，并将结果写入变量 `P1` | 无 |
| 异或 | `XOR P1 P2 P3` | 计算变量 `P2` ^ 变量 `P3` 的值，并将结果写入变量 `P1` | 无 |
| 左移 | `LMV P1 P2 P3` | 计算变量 `P2` << 变量 `P3` 的值，并将结果写入变量 `P1` | 无 |
| 右移 | `RMV P1 P2 P3` | 计算变量 `P2` >> 变量 `P3` 的值，并将结果写入变量 `P1` | 无 |
| 比较 | `CMP P1 P2 P3` | 判断变量 `P2` 是否小于变量 `P3` 的值，成立则为1，反之则为0，并将结果写入变量 `P1` | 无 |
| 输入 | `IPT P1` | 输入一个字符，并写入变量 `P1` | 无 |
| 输入 | `PRT P1` | 以一个字符的形式将变量 `P1` 作为 ASCII 码输出 | 无 |
| 清屏 | `CLS` | 输出24个换行（没错就是这么敷衍） | 无 |
| 设置锚点 | `SPT P1` | 将下一条非预解释指令设置为锚点 `P1` | 本条指令为预解释指令 |
| 跳转 | `GTO P1` | 跳转到锚点 `P1` | 锚点 `P1` 必须存在，否则报错 |
| 读取并跳转 | `GTW P1 P2 P3 P4` | 读取变量 `P1` 的值作为锚点最低2位，变量 `P2` 的值作为锚点次低2位，变量 `P3` 的值作为锚点次高2位，变量 `P4` 的值作为锚点最高2位，并跳转到锚点 | 锚点必须存在，否则报错 |
| 判断并跳转 | `IFG P1 P2` | 如果变量 `P1` 为0，则跳转到锚点 `P2`，反之则继续执行 | 锚点 `P2` 必须存在，否则报错 |
| 读变量 | `RMM P1 P2 P3 P4 P5` | 读取变量 `P2` 的值作为指针最低2位，变量 `P3` 的值作为指针次低2位，变量 `P4` 的值作为指针次高2位，变量 `P5` 的值作为指针最高2位，并将指针读取出的值写入变量 `P1` | 无 |
| 读变量 | `RMM P1 P2 P3 P4 P5` | 读取变量 `P1` 的值作为指针最低2位，变量 `P2` 的值作为指针次低2位，变量 `P3` 的值作为指针次高2位，变量 `P4` 的值作为指针最高2位，并将变量 `P1` 的值写入指针指向的变量 | 无 |
| 复制 | `IFG P1 P2` | 如果变量 `P1` 设置为变量 `P2`的值 | 无 |
| 释放 | `DEL P1` | 释放变量 `P1` | 无 |
| 终止 | `END` | 释放所有变量并终止程序 | 无 |
