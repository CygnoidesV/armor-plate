
#include "flash.h"

void Flash_Erase(uint32_t addr)
{
	FLASH_EraseInitTypeDef My_Flash;  //����FLASH_EraseInitTypeDef �ṹ��Ϊ My_Flash
	HAL_FLASH_Unlock(); //����Flash
            
	My_Flash.TypeErase = FLASH_TYPEERASE_PAGES;  //����Flashִ��ҳ��ֻ����������
	My_Flash.PageAddress = addr;  //����Ҫ�����ĵ�ַ
	My_Flash.NbPages = 1; //˵��Ҫ������ҳ�����˲���������Min_Data = 1��Max_Data =(���ҳ��-��ʼҳ��ֵ)֮���ֵ
					
	uint32_t PageError = 0;  //����PageError,������ִ�����������ᱻ����Ϊ�����FLASH��ַ
	HAL_FLASHEx_Erase(&My_Flash, &PageError);  //���ò�����������
	HAL_FLASH_Lock();
}

void Flash_Write16(uint32_t addr, uint16_t data)
{
	HAL_FLASH_Unlock(); //����Flash
	FLASH_EraseInitTypeDef My_Flash;  //����FLASH_EraseInitTypeDef �ṹ��Ϊ My_Flash
            
	My_Flash.TypeErase = FLASH_TYPEERASE_PAGES;  //����Flashִ��ҳ��ֻ����������
	My_Flash.PageAddress = addr;  //����Ҫ�����ĵ�ַ
	My_Flash.NbPages = 1; //˵��Ҫ������ҳ�����˲���������Min_Data = 1��Max_Data =(���ҳ��-��ʼҳ��ֵ)֮���ֵ
					
	uint32_t PageError = 0;  //����PageError,������ִ�����������ᱻ����Ϊ�����FLASH��ַ
	HAL_FLASHEx_Erase(&My_Flash, &PageError);  //���ò�����������
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr, data); //��Flash������д
	HAL_FLASH_Lock(); //��סFlash
}

void Flash_Write32(uint32_t addr, uint32_t data)
{
	HAL_FLASH_Unlock(); //����Flash
	FLASH_EraseInitTypeDef My_Flash;  //����FLASH_EraseInitTypeDef �ṹ��Ϊ My_Flash
            
	My_Flash.TypeErase = FLASH_TYPEERASE_PAGES;  //����Flashִ��ҳ��ֻ����������
	My_Flash.PageAddress = addr;  //����Ҫ�����ĵ�ַ
	My_Flash.NbPages = 1; //˵��Ҫ������ҳ�����˲���������Min_Data = 1��Max_Data =(���ҳ��-��ʼҳ��ֵ)֮���ֵ
					
	uint32_t PageError = 0;  //����PageError,������ִ�����������ᱻ����Ϊ�����FLASH��ַ
	HAL_FLASHEx_Erase(&My_Flash, &PageError);  //���ò�����������
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, data); //��Flash������д
	HAL_FLASH_Lock(); //��סFlash
}
