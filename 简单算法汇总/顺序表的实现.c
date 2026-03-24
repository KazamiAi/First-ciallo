#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100  // ˳������������
// ˳����ṹ��
typedef struct {
    int data[MAX_SIZE];  // �洢���ݵ�����
    int length;          // ˳�����ǰ����
} SeqList;
// ��ʼ��˳���
void initSeqList(SeqList* list) {
    list->length = 0;
}
// ����Ԫ�ص�˳���
int insert(SeqList* list, int index, int value) {
if (index < 0 || index > list->length || list->length >= MAX_SIZE) return -1;
for (int i = list->length; i > index; i--) list->data[i] = list->data[i - 1];
list->data[index] = value;
list->length++;
return 0;
}
// ɾ��ָ��λ�õ�Ԫ��
int delete(SeqList* list, int index) {
if (index < 0 || index >= list->length) return -1;
for (int i = index; i < list->length - 1; i++) list->data[i] = list->data[i + 1];
list->length--;
return 0;
}
// ����ָ��Ԫ�ص�λ��
int find(SeqList* list, int value) {
for (int i = 0; i < list->length; i++) if (list->data[i] == value) return i;
return -1;
}
// ��ӡ˳���
void printSeqList(SeqList* list) {
if (list->length == 0) printf("˳���Ϊ��\n");
else {
for (int i = 0; i < list->length; i++) printf("%d ", list->data[i]);
printf("\n");
}
}
int main() {
SeqList list;
initSeqList(&list);
int choice, index, value;
while (1) {
printf("\n1. ����Ԫ��\n2. ɾ��Ԫ��\n3. ����Ԫ��\n4. ��ӡ˳���\n5. �˳�\nѡ��: ");
scanf("%d", &choice);
switch (choice) {
case 1:
printf("����λ��(0~%d): ", list.length);
scanf("%d", &index);
printf("Ԫ��ֵ: ");
scanf("%d", &value);
if (insert(&list, index, value) == 0) printf("����ɹ���\n");
else printf("����ʧ�ܣ�\n");
break;
case 2:
printf("ɾ��λ��(0~%d): ", list.length - 1);
scanf("%d", &index);
if (delete(&list, index) == 0) printf("ɾ���ɹ���\n");
else printf("ɾ��ʧ�ܣ�\n");
break;
case 3:
printf("����Ԫ��: ");
scanf("%d", &value);
index = find(&list, value);
if (index != -1) printf("Ԫ�� %d ��λ�� %d\n", value, index);
else printf("Ԫ��δ�ҵ�\n");
break;
case 4:
printSeqList(&list);
break;
case 5:
return 0;
default:
printf("��Чѡ������������\n");
}
}
return 0;
}

