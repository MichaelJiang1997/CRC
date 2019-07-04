// CRC.cpp 

#include <iostream>
using namespace std;

//CRC编码
string crc_encode(string raw_data, string gx)
{
	string crc_data = raw_data;
	//在原生数据的后面追加gx长度-1个0
	for (uint8_t i = 0; i < gx.length() - 1; i++)
	{
		raw_data += "0";
	}

	string quo = "";//存放商

	//按位异或
	for (uint8_t i = 0; i < raw_data.length(); i++)
	{
		//如果最高位为1则上1
		if (raw_data[i] == '1')
		{
			quo += '1';
			for (uint8_t j = 0; j < gx.length(); j++)
			{
				if ((raw_data[i + j] - 48) ^ (gx[j] - 48))
				{
					raw_data[i + j] = '1';
				}
				else
				{
					raw_data[i + j] = '0';
				}
			}
		}
		//如果最高位为0则上0
		else
		{
			quo += '0';
			for (uint8_t j = 0; j < gx.length(); j++)
			{
				if ((raw_data[i + j] - 48) ^ 0)
				{
					raw_data[i + j] = '1';
				}
				else
				{
					raw_data[i + j] = '0';
				}
			}
		}
		//结束出操作标志
		if (quo.length() == raw_data.length() - gx.length() + 1)
		{
			break;
		}
	}
	//此时CRC校验码已经在raw_data的末尾，只要拼接到crc_data后即可
	crc_data += raw_data.substr(crc_data.length(), gx.length() - 1);
	return crc_data;
}

//CRC解码
//rem为余数 引用传递，当检查有错时函数返回false且rem值为含有1的余数字符串
//可以根据余数rem的值纠错，但这取决于gx所以无法在此函数中直接纠正
bool crc_check(string crc_data, string gx, string &rem)
{

	string quo = "";//存放商
	//按位异或
	for (uint8_t i = 0; i < crc_data.length(); i++)
	{
		//如果最高位为1则上1
		if (crc_data[i] == '1')
		{
			quo += '1';
			for (uint8_t j = 0; j < gx.length(); j++)
			{
				if ((crc_data[i + j] - 48) ^ (gx[j] - 48))
				{
					crc_data[i + j] = '1';
				}
				else
				{
					crc_data[i + j] = '0';
				}
			}
		}
		//如果最高位为0则上0
		else
		{
			quo += '0';
			for (uint8_t j = 0; j < gx.length(); j++)
			{
				if ((crc_data[i + j] - 48) ^ 0)
				{
					crc_data[i + j] = '1';
				}
				else
				{
					crc_data[i + j] = '0';
				}
			}
		}
		//结束出操作标志
		if (quo.length() == crc_data.length() - gx.length() + 1)
		{
			break;
		}
	}
	rem = crc_data.substr(crc_data.length() - gx.length()+1, gx.length()-1);

	//此时检查rem的值是否全为0;如果全为0则校验通过否则出错
	bool error = false;
	for (uint8_t i = 0; i < rem.length(); i++)
	{
		if (rem[i] != '0')
		{
			error = true;
			break;
		}
	}
	if (error)
		return false;
	else
		return true;
}

int main()
{
	string raw_data = "110";
	string gx = "11011";
	string rem = "";
	cout << "Example 1:" << endl;
	cout << "Raw data:" << raw_data << endl;
	cout << "Gx:" << gx << endl;
	cout << "CRC Encode:" <<crc_encode(raw_data, gx) << endl;
	cout << "Check Result:"<<crc_check(crc_encode(raw_data, gx), gx, rem) << endl;
	cout << "Rem:" << rem << endl;

	raw_data = "1100";
	gx = "1011";
	cout << "\n";
	cout << "Example 2:" << endl;
	cout << "Raw data:" << raw_data << endl;
	cout << "Gx:" << gx << endl;
	cout << "CRC Encode:" << crc_encode(raw_data, gx) << endl;
	cout << "Check Result:" << crc_check("1100011", gx, rem) << endl;
	cout << "Rem:" << rem << endl;
}