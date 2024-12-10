#pragma once
#include "../acl_cpp_define.hpp"
#include <list>
#include <vector>
#include "dbuf_pool.hpp"
#include "pipe_stream.hpp"

struct ACL_JSON_NODE;
struct ACL_JSON;
struct ACL_ITER;

/**
 * �� ACL ���� json ������ķ�װ������ C++ �û�ʹ�ã������̫ע���������أ�
 * ����ֱ��ʹ�ø��࣬����ڷ����ִ���ҷǳ�ע�����ܣ�����ֱ��ʹ�� ACL ���
 * json ����������Ϊ����Ҳ�ǵ����� ACL ���е� json �������̣������ж��ο���
 * ���̣����ܻ���΢Ӱ��һЩ���ܣ�������һ���Ӧ�����Ӱ����΢�������
 * ���� json �����丽�� json �ڵ������ǹ����ڲ��� dbuf ������, �������ڲ�
 * ������ json �ڵ��������� json ��������ʱһ���ͷ�.
 */

namespace acl {

class string;
class json;

/**
 * json �ڵ㣬������������ json.create_node() ��ʽ����
 */
class ACL_CPP_API json_node : public dbuf_obj {
public:
	/**
	 * ȡ�ñ� json �ڵ�ı�ǩ��
	 * @return {const char*} ���� json �ڵ��ǩ����������ؿգ���˵��
	 *  ��������Ҫ�жϷ���ֵ
	 */
	const char* tag_name() const;

	/**
	 * ���ظ� json �ڵ���ı���ǩֵ������ֵΪ�����ͻ���ֵ��ʱ�����߿�
	 * ���н���ת��
	 * @return {const char*} ���ؿ�˵��û���ı���ǩֵ
	 */
	const char* get_text() const;

	/**
	 * ���� json �ڵ�����ӽڵ�ʱ�����ر� json �ڵ��ǩ��Ӧ�� json �ӽڵ�
	 * @return {const json_node*} ���� NULL ˵���������ӽڵ�
	 *  ע��get_text �� get_obj ����ͬʱ���ط� NULL
	 */
	json_node* get_obj() const;

	/**
	 * �� json �ڵ�Ϊ�ַ�������ʱ���ú��������ַ�������
	 * @return {const char*} ���� NULL ��ʾ�ýڵ���ַ�������
	 */
	const char* get_string() const;

	/**
	 * �� json �ڵ�Ϊ����������ʱ���ú������س�����ֵ��ָ���ַ
	 * @return {const long long int*} ������ NULL ʱ��ʾ�ö���ǳ���������
	 */
#if defined(_WIN32) || defined(_WIN64)
	const __int64* get_int64(void) const;
#else
	const long long int* get_int64() const;
#endif

	/**
	 * �� json �ڵ�Ϊ��������ʱ���ú������س�����ֵ��ָ���ַ
	 * @return {const double*} ������ NULL ʱ��ʾ�ö���Ǹ�������
	 */
	const double *get_double() const;

	/**
	 * �� json �ڵ�Ϊ��������ʱ���ú������ز���ֵ��ָ���ַ
	 * @return {bool*} ������ NULL ʱ��ʾ�ö���ǲ�������
	 */
	const bool* get_bool() const;

	/**
	 * �жϱ��ڵ������Ƿ�Ϊ�ַ�������
	 * @return {bool}
	 */
	bool is_string() const;

	/**
	 * �жϱ��ڵ������Ƿ�Ϊ��������
	 * @return {bool}
	 */
	bool is_number() const;

	/**
	 * �жϱ��ڵ������Ƿ�Ϊ��������
	 * @return {bool}
	 */
	bool is_double() const;

	/**
	 * �жϱ��ڵ������Ƿ�Ϊ��������
	 * @return {bool}
	 */
	bool is_bool() const;

	/**
	 * �жϱ��ڵ������Ƿ�Ϊ null ����
	 * @return {bool}
	 */
	bool is_null() const;

	/**
	 * �жϱ��ڵ��Ƿ�Ϊ��������
	 * @return {bool}
	 */
	bool is_object() const;

	/**
	 * �жϱ��ڵ��Ƿ�Ϊ��������
	 * @return {bool}
	 */
	bool is_array() const;

	/**
	 * ��øýڵ����͵�����
	 * @return {const char*}
	 */
	const char* get_type() const;

	/**
	 * ���� json �ڵ��б�ǩʱ�������������µı�ǩֵ���Ǿɵı�ǩ��
	 * @param name {const char*} �µı�ǩֵ��Ϊ�ǿ��ַ���
	 * @return {bool} ���� false ��ʾ�ýڵ�û�б�ǩ������մ���û�н����滻
	 */
	bool set_tag(const char* name);

	/**
	 * ���� json �ڵ�ΪҶ�ڵ�ʱ�������������滻�ڵ���ı�ֵ
	 * @param text {const char*} �µ�Ҷ�ڵ��ı�ֵ��Ϊ�ǿ��ַ���
	 * @return {bool} ���� false ��ʾ�ýڵ��Ҷ�ڵ������Ƿ�
	 */
	bool set_text(const char* text);

	/**
	 * ����ǰ json �ڵ�ת���� json �ַ���(������ json �ڵ㼰���ӽڵ�)
	 * @param out {string*} �ǿ�ʱ����ʹ�ô˻�����������ʹ���ڲ�������
	 * @return {const char*}
	 */
	const string& to_string(string* out = NULL) const;

	/////////////////////////////////////////////////////////////////////

	/**
	 * ���� json �ڵ����� json_node �ӽڵ����
	 * @param child {json_node*} �ӽڵ����
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�����ӽڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_child(json_node* child, bool return_child = false);

	/**
	 * ���� json �ڵ����� json_node �ӽڵ����
	 * @param child {json_node&} �ӽڵ����
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�����ӽڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_child(json_node& child, bool return_child = false);

	/**
	 * ����һ�� json �ڵ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param as_array {bool} �Ƿ��������
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_child(bool as_array = false, bool return_child = false);
	json_node& add_array(bool return_child = false);

	/**
	 * ����һ�� json �ڵ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param tag {const char*} ��ǩ��
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_child(const char* tag, bool return_child = false);

	/**
	 * ����һ�� json �ڵ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param tag {const char*} ��ǩ��
	 * @param node {json_node*} ��ǩֵָ��
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_child(const char* tag, json_node* node,
		bool return_child = false);

	/**
	 * ����һ�� json �ڵ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param tag {const char*} ��ǩ��
	 * @param node {json_node&} ��ǩֵ����
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_child(const char* tag, json_node& node,
		bool return_child = false);

	/**
	 * ����һ���ַ������͵� json �ڵ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param tag {const char*} ��ǩ��
	 * @param value {const char*} ��ǩֵ
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 * ע���˴��� add_text �� add_child ��ͬ���Ĺ���
	 */
	json_node& add_text(const char* tag, const char* value,
		bool return_child = false);

	/**
	 * ����һ��int64 ���͵� json �ڵ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param tag {const char*} ��ǩ��
	 * @param value {int64} ��ǩֵ
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
#if defined(_WIN32) || defined(_WIN64)
	json_node& add_number(const char* tag, __int64 value,
		bool return_child = false);
#else
	json_node& add_number(const char* tag, long long int value,
		bool return_child = false);
#endif

	/**
	 * ����һ�� double ���͵� json �ڵ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param tag {const char*} ��ǩ��
	 * @param value {double} ��ǩֵ
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_double(const char* tag, double value,
		bool return_child = false);

	/**
	 * ����һ�� double ���͵� json �ڵ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param tag {const char*} ��ǩ��
	 * @param value {double} ��ǩֵ
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @param precision {int} С���㾫�ȣ����� 0 ʱ��Ч������ȡȱʡֵΪ 4
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_double(const char* tag, double value, int precision,
		bool return_child = false);

	/**
	 * ����һ���������͵� json �ڵ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param tag {const char*} ��ǩ��
	 * @param value {bool} ��ǩֵ
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_bool(const char* tag, bool value,
		bool return_child = false);

	/**
	 * ����һ�� null ���͵� json �ڵ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param tag {const char*} ��ǩ��
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_null(const char* tag, bool return_child = false);

	/**
	 * ����һ�� json �ַ������󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param text {const char*} �ı��ַ���
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_array_text(const char* text, bool return_child = false);

	/**
	 * ����һ�� json ���ֶ��󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param value {acl_int64} ����ֵ
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
#if defined(_WIN32) || defined(_WIN64)
	json_node& add_array_number(__int64 value,
		bool return_child = false);
#else
	json_node& add_array_number(long long int value, bool return_child = false);
#endif

	/**
	 * ����һ�� json double ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param value {double} ֵ
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_array_double(double value, bool return_child = false);

	/**
	 * ����һ�� json �������󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param value {bool} ����ֵ
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_array_bool(bool value, bool return_child = false);

	/**
	 * ����һ�� json null ���󣬲���֮����Ϊ�� json �ڵ���ӽڵ�
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {json_node&} return_child Ϊ true ʱ�������½ڵ�����ã�
	 *  ���򷵻ر� json �ڵ���������
	 */
	json_node& add_array_null(bool return_child = false);

	/**
	 * @return {json_node&} ���ر��ڵ�ĸ��ڵ����ã��ڲ��ü�����ʽ���� json
	 *  ����ʱ���������������ڷ��ظ��ڵ�
	 */
	json_node& get_parent() const;

	/////////////////////////////////////////////////////////////////////

	/**
	 * ��ñ��ڵ�ĵ�һ���ӽڵ㣬��Ҫ�����ӽڵ�ʱ�������ȵ��ô˺���
	 * @return {json_node*} ���ؿձ�ʾû���ӽڵ㣬���صķǿն�����
	 *  ���ⲿ delete����Ϊ�ڲ����Զ��ͷ�
	 */
	json_node* first_child();

	/**
	 * ��ñ��ڵ����һ���ӽڵ�
	 * @return {json_node*} ���ؿձ�ʾ�������̽��������صķǿն�����
	 *  ���ⲿ delete����Ϊ�ڲ����Զ��ͷ�
	 */
	json_node* next_child();

	/**
	 * �ڱ����ӽڵ������ɾ���ͷ�ָ�����ӽڵ㣬��������һ���ӽڵ�
	 * @param child {json_node*} ���������е��� first_child/next_child/remove_child
	 *  ʱ���صĵ�ǰ json �ڵ���ӽڵ㣬�ýڵ㽫���� Json ������ɾ�����ͷ�
	 * @return {json_node*} ���ص�ǰ�ڵ����һ���ӽڵ�
	 */
	json_node* free_child(json_node* child);

	/**
	 * �ӵ�ǰ json �ڵ���ӽڵ�����ȡ��Ӧ��ǩ�� json �ӽڵ�
	 * @param tag {const char*} json �ӽڵ�ı�ǩ��
	 * @return {json_node*} ���� NULL ��ʾ������
	 */
	json_node* operator[] (const char* tag);

	/**
	 * ���ظ� json �ڵ������� json ���е����
	 * @return {int}
	 */
	int   depth() const;

	/**
	 * ���ظ� json �ڵ����һ���ӽڵ�ĸ���
	 * @return {int} ��Զ >= 0
	 */
	int   children_count() const;

	/**
	 * �����ڵ㼰���ӽڵ�� json ����ɾ�������ڴ潫�� json ����ͳһ�ͷ�
	 * @return {int} ���ͷŵĽڵ�����
	 */
	int detach();

	/**
	 * ��ֹ��ǰ json �ڵ㣬������ json �ַ���ʱ����ֹ�Ľڵ㽫�����ԣ����ýڵ㲢δ
	 * �� json �ڵ�����ɾ�����������´α���ʱ�����������øýڵ�
	 * @param yes {bool} �Ƿ��ֹ�� json �ڵ㣬false ��ʾ���ã�true ��ʾ����
	 */
	void disable(bool yes);

	/**
	 * �жϵ�ǰ json �ڵ��Ƿ񱻽�ֹ��
	 * @return {bool}
	 */
	bool disabled() const;

	/**
	 * ���ڱ����� json �ڵ�ʱ���ڲ��ᶯ̬����һЩ��ʱ json_node ���󣬵���
	 * �˺������������Щ����һ�����ô˺������������������ first_child,
	 * next_child ���ص� json_node �ڵ���󽫲��ٿ��ã����������ڴ�Ƿ�
	 * ����
	 */
	void clear();

	/**
	 * ��� json ���������
	 * @return {json&}
	 */
	json& get_json() const;

	/**
	 * ȡ����Ӧ�� ACL ���е� json �ڵ����
	 * @return {ACL_JSON_NODE*} ���ؽڵ����ע���ýڵ��û����ܵ����ͷ�
	 */
	ACL_JSON_NODE* get_json_node() const;

private:
	friend class json;
	friend class dbuf_guard;

	/**
	 * ���캯����Ҫ��ö������ֻ���� json ���󴴽�
	 * @param node {ACL_JSON_NODE*} ACL ���е� ACL_JSON_NODE �ṹ����
	 */
	json_node(ACL_JSON_NODE* node, json* json_ptr);

	/**
	 * Ҫ��ö�������Ƕ�̬������
	 */
	~json_node();

	/**
	 * ���� json �ڵ�
	 * @param node {ACL_JSON_NODE*}
	 */
	void set_json_node(ACL_JSON_NODE* node);

private:
	ACL_JSON_NODE* node_me_;
	json* json_;
	dbuf_guard* dbuf_;
	json_node* parent_;
	ACL_ITER* iter_;
	string* buf_;
	json_node* obj_;

	union {
#if defined(_WIN32) || defined(_WIN64)
		__int64 n;
#else
		long long int n;
#endif
		bool   b;
		double d;
	} node_val_;
};

class ACL_CPP_API json : public pipe_stream, public dbuf_obj {
public:
	/**
	 * ���캯���������ڽ��� json �ַ��������� json ����
	 * @param data {const char*} json ��ʽ���ַ�����������������
	 *  json �ַ�����Ҳ�����ǲ��ֵ� json �ַ�����Ҳ�����ǿ�ָ�룬
	 *  ������Σ��û���Ȼ�����ò��ֻ������� json �ַ������� update
	 *  �������ڵ��� update �����н��� json����ʵ�������캯����
	 *  �� data �����ǿ�ʱ����Ҳ����� update
	 * @param dbuf {dbuf_guard*} �ǿ�ʱ����Ϊ�ڴ�ع������󣬷����ڲ�
	 *  ���Զ�����һ���ڴ�ع�������
	 */
	json(const char* data = NULL, dbuf_guard* dbuf = NULL);

	/**
	 * ����һ�� json �����е�һ�� json �ڵ㹹��һ���µ� json ����
	 * @param node {const json_node&} Դ json �����е�һ�� json �ڵ�
	 * @param dbuf {dbuf_guard*} �ǿ�ʱ����Ϊ�ڴ�ع������󣬷����ڲ�
	 *  ���Զ�����һ���ڴ�ع�������
	 */
	json(const json_node& node, dbuf_guard* dbuf = NULL);

	~json();

	/**
	 * �����Ƿ��ڽ���ʱ�Զ�����������ֵ�����
	 * @param on {bool}
	 * @return {json&}
	 */
	json& part_word(bool on);

	/**
	 * ����ʽ��ʽѭ�����ñ��������� json ���ݣ�Ҳ����һ��������
	 * ������ json ���ݣ�������ظ�ʹ�ø� json ������������� json
	 * ������Ӧ���ڽ�����һ�� json ����ǰ���� reset() ��������
	 * ����һ�εĽ������
	 * @param data {const char*} json ����
	 @return {const char*} �����������󣬸÷���ֵ��ʾʣ�����ݵ�ָ���ַ
	 */
	const char* update(const char* data);

	/**
	 * �ж��Ƿ�������
	 * @return {bool}
	 */
	bool finish();

	/**
	 * ���� json ������״̬���� json ������������Զ�� json ����
	 * ���н������ڷ���ʹ�ñ� json ������ǰ����Ҫ���ñ���������
	 * �ڲ� json ������״̬�������һ�εĽ������
	 */
	void reset();

	/**
	 * �� json ������ȡ��ĳ����ǩ���ĵ�һ���ڵ�
	 * @param tag {const char*} ��ǩ��(�����ִ�Сд)
	 * @return {json_node*} ���� json �ڵ���󣬲������򷵻� NULL
	 *  ע�����ص� json_node �ڵ�����޸ģ�������ɾ���ڵ㣬�ڲ����Զ�ɾ�����ƣ�
	 *  �����÷��� clear/getElementsByTagName/getElementsByTags �󣬽ڵ�
	 *  �����ٱ����ã���Ϊ�ڵ���ڴ汻�Զ��ͷ�
	 */
	json_node* getFirstElementByTagName(const char* tag) const;

	/**
	 * �����������ֱ�ӻ�ö�Ӧ��ǩ���ĵ�һ���ڵ�
	 * @param tag {const char*} ��ǩ��(�����ִ�Сд)
	 * @return {json_node*} ���� json �ڵ���󣬲������򷵻� NULL
	 *  ע�����ص� json_node �ڵ�����޸ģ�������ɾ���ڵ㣬�ڲ����Զ�ɾ�����ƣ�
	 *  �����÷��� clear/getElementsByTagName/getElementsByTags �󣬽ڵ�
	 *  �����ٱ����ã���Ϊ�ڵ���ڴ汻�Զ��ͷ�
	 */
	json_node* operator[](const char* tag) const;

	/**
	 * �� json ������ȡ��ĳ����ǩ�������нڵ㼯��
	 * @param tag {const char*} ��ǩ��(�����ִ�Сд)
	 * @return {const std::vector<json_node*>&} ���ؽ�����Ķ������ã�
	 *  �����ѯ���Ϊ�գ���ü���Ϊ�գ�����empty() == true
	 *  ע�����ص� json_node �ڵ�����޸ģ�������ɾ���ڵ㣬�ڲ����Զ�ɾ�����ƣ�
	 *  �����÷��� clear/getElementsByTagName/getElementsByTags �󣬽ڵ�
	 *  �����ٱ����ã���Ϊ�ڵ���ڴ汻�Զ��ͷ�
	 */
	const std::vector<json_node*>& getElementsByTagName(const char* tag) const;

	/**
	 * �� json �����л�����е�������༶��ǩ����ͬ�� json �ڵ�ļ���
	 * @param tags {const char*} �༶��ǩ������ '/' �ָ�������ǩ����
	 *  ����� json ���ݣ�
	 *  { 'root': [
	 *      'first': { 'second': { 'third': 'test1' } },
	 *      'first': { 'second': { 'third': 'test2' } },
	 *      'first': { 'second': { 'third': 'test3' } }
	 *    ]
	 *  }
	 *  ����ͨ���༶��ǩ����root/first/second/third һ���Բ�����з���
	 *  �����Ľڵ�
	 * @return {const std::vector<json_node*>&} ���������� json �ڵ㼯��, 
	 *  �����ѯ���Ϊ�գ���ü���Ϊ�գ�����empty() == true
	 *  ע�����ص� json_node �ڵ�����޸ģ�������ɾ���ڵ㣬�ڲ����Զ�ɾ�����ƣ�
	 *  �����÷��� clear/getElementsByTagName/getElementsByTags �󣬽ڵ�
	 *  �����ٱ����ã���Ϊ�ڵ���ڴ汻�Զ��ͷ�
	 */
	const std::vector<json_node*>& getElementsByTags(const char* tags) const;

	/**
	 * �� json �����л�����е�������༶��ǩ����ͬ�� json �ڵ�ļ���
	 * @param tags {const char*} �༶��ǩ������ '/' �ָ�������ǩ����
	 *  ����� json ���ݣ�
	 *  { 'root': [
	 *      'first': { 'second': { 'third': 'test1' } },
	 *      'first': { 'second': { 'third': 'test2' } },
	 *      'first': { 'second': { 'third': 'test3' } }
	 *    ]
	 *  }
	 *  ����ͨ���༶��ǩ����root/first/second/third һ���Բ�����з���
	 *  �����Ľڵ�
	 * @return {json_node*} ���� NULL ��ʾ������
	 */
	json_node* getFirstElementByTags(const char* tags) const;

	/**
	 * ȡ�� acl ���е� ACL_JSON ����
	 * @return {ACL_JSON*} ��ֵ������Ϊ�գ�ע���û������޸ĸö����ֵ��
	 *  ���������ͷŸö���
	 */
	ACL_JSON* get_json() const;

	/////////////////////////////////////////////////////////////////////

	/**
	 * ����һ�� json_node Ҷ�ڵ���󣬸ýڵ����ĸ�ʽΪ��
	 * "tag_name": "tag_value"
	 * @param tag {const char*} ��ǩ��
	 * @param value {const char*} ��ǩֵ
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 *  ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_node(const char* tag, const char* value);

	/**
	 * ����һ�� json_node Ҷ�ڵ���󣬸ýڵ����ĸ�ʽΪ��
	 * "tag_name": tag_value
	 * @param tag {const char*} ��ǩ��
	 * @param value {int64} ��ǩֵ
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 *  ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
#if defined(_WIN32) || defined(_WIN64)
	json_node& create_node(const char* tag, __int64 value);
#else
	json_node& create_node(const char* tag, long long int value);
#endif

	/**
	 * ����һ�� json_node Ҷ�ڵ���󣬸ýڵ����ĸ�ʽΪ��
	 * "tag_name": tag_value
	 * @param tag {const char*} ��ǩ��
	 * @param value {double} ��ǩֵ
	 * @param precision {int} С�������ľ��ȣ�> 0 ʱ��Ч��������Ϊ 4��
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 *  ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_double(const char* tag, double value, int precision = 4);

	/**
	 * ����һ�� json_node Ҷ�ڵ���󣬸ýڵ����ĸ�ʽΪ��
	 * "tag_name": true|false
	 * @param tag {const char*} ��ǩ��
	 * @param value {bool} ��ǩֵ
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 *  ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_node(const char* tag, bool value);

	/**
	 * ����һ�� json_node null Ҷ�ڵ���󣬸ýڵ����ĸ�ʽΪ��
	 * "tag_name": null
	 * @param tag {const char*} ��ǩ��
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 *  ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_null(const char* tag);

	/**
	 * ����һ�� json_node Ҷ�ڵ��ַ������󣬸ýڵ����ĸ�ʽΪ��"string"
	 * �� json �淶���ýڵ�ֻ�ܼ��������������
	 * @param text {const char*} �ı��ַ���
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 *  ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_array_text(const char* text);

	/**
	 * ����һ�� json_node Ҷ�ڵ���ֵ����
	 * �� json �淶���ýڵ�ֻ�ܼ��������������
	 * @param value {acl_int64} ��ֵ
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 * ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
#if defined(_WIN32) || defined(_WIN64)
	json_node& create_array_number(__int64 value);
#else
	json_node& create_array_number(long long int value);
#endif

	/**
	 * ����һ�� json_node Ҷ�ڵ���ֵ����
	 * �� json �淶���ýڵ�ֻ�ܼ��������������
	 * @param value {double} ֵ
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 * ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_array_double(double value);

	/**
	 * ����һ�� json_node Ҷ�ڵ㲼������
	 * �� json �淶���ýڵ�ֻ�ܼ��������������
	 * @param value {bool} ����ֵ
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 * ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_array_bool(bool value);

	/**
	 * ����һ�� json_node Ҷ�ڵ� null ����
	 * �� json �淶���ýڵ�ֻ�ܼ��������������
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 * ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_array_null();

	/**
	 * ����һ�� json_node �ڵ��������󣬸ö���û�б�ǩ,
	 * �ýڵ����ĸ�ʽΪ��"{}" ��������� "[]"
	 * @param as_array {bool} �Ƿ��������
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 *  ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_node(bool as_array = false);
	json_node& create_array();

	/**
	 * ����һ�� json_node �ڵ���󣬸ýڵ����ĸ�ʽΪ��tag_name: {}
	 * �� tag_name: []
	 * @param tag {const char*} ��ǩ��
	 * @param node {json_node*} json �ڵ������Ϊ��ǩֵ
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 *  ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_node(const char* tag, json_node* node);

	/**
	 * ����һ�� json_node �ڵ���󣬸ýڵ����ĸ�ʽΪ��tag_name: {}
	 * �� tag_name: []
	 * @param tag {const char*} ��ǩ��
	 * @param node {json_node&} json �ڵ������Ϊ��ǩֵ
	 * @return {json_node&} �²����� json_node ������Ҫ�û��ֹ��ͷţ�
	 *  ��Ϊ�� json �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ������
	 *  ����ʱ���� reset ���ͷ���Щ json_node �ڵ����
	 */
	json_node& create_node(const char* tag, json_node& node);

	/**
	 * ��ָ���� json �ڵ�� json ������ɾ��, ͬʱ�ͷ����ڴ���Դ, ���ñ�����
	 * ��Ӧ�ý�������ʹ�ø� json �ڵ����.
	 * @param node {json_node*} �� json �ڵ㼰��������ӽڵ㽫��� json ��
	 *  ����ɾ��, ����ռ�õ��ڴ���Դ������ json ��������ʱ�� dbuf ����.
	 */
	void remove(json_node* node);

	/////////////////////////////////////////////////////////////////////

	/**
	 * ��һ�� json �����е�һ�� json �ڵ㸴������һ json �����е�һ��
	 * �� json �ڵ��в����ظ��µ� json �ڵ�
	 * @param node {json_node*} Դ json �����һ�� json �ڵ�
	 * @return {json_node&} ��ǰĿ�� json �������´����� json �ڵ�
	 */
	json_node& duplicate_node(const json_node* node);

	/**
	 * ��һ�� json �����е�һ�� json �ڵ㸴������һ json �����е�һ��
	 * �� json �ڵ��в����ظ��µ� json �ڵ�
	 * @param node {json_node&} Դ json �����һ�� json �ڵ�
	 * @return {json_node&} ��ǰĿ�� json �������´����� json �ڵ�
	 */
	json_node& duplicate_node(const json_node& node);

	/////////////////////////////////////////////////////////////////////

	/**
	 * ��ø��ڵ����
	 * @return {json_node&}
	 */
	json_node& get_root();

	/**
	 * ��ʼ������ json ���󲢻�õ�һ���ڵ�
	 * @return {json_node*} ���ؿձ�ʾ�� json ����Ϊ�սڵ�
	 *  ע�����صĽڵ�����û������ֹ��ͷţ���Ϊ�ö���
	 *  �ڲ����Զ��ͷ�
	 */
	json_node* first_node();

	/**
	 * ������ json �������һ�� json �ڵ�
	 * @return {json_node*} ���ؿձ�ʾ�������
	 *  ע�����صĽڵ�����û������ֹ��ͷţ���Ϊ�ö���
	 *  �ڲ����Զ��ͷ�
	 */
	json_node* next_node();

	/**
	 * �ڱ�������ͨ��������ɾ����ǰ�������� json �ڵ㣬��������һ�� json �ڵ�
	 * @param curr {json_node*} �� first_node/next_node/free_node ���صĽڵ�
	 * @return {json_node*} ������һ�� json �ڵ�
	 */
	json_node* free_node(json_node* curr);

	/**
	 * �� json ������ת���ַ���
	 * @param out {string&} �洢ת������Ļ�����
	 * @param add_space {bool} ���� json ʱ�Ƿ��Զ��ڷָ����������ӿո�
	 */
	void build_json(string& out, bool add_space = false) const;

	/**
	 * �� json ������ת���� json �ַ���
	 * @param out {string*} �ǿ�ʱ����ʹ�ô˻�����������ʹ���ڲ�������
	 * @param add_space {bool} ���� json ʱ�Ƿ��Զ��ڷָ����������ӿո�
	 * @return {const string&}
	 */
	const string& to_string(string* out = NULL, bool add_space = false) const;

	/**
	 * ����ڴ�ض���ָ��
	 * @return {dbuf_guard*}
	 */
	dbuf_guard* get_dbuf() const {
		return dbuf_;
	}

	// pipe_stream �麯������

	virtual int push_pop(const char* in, size_t len,
		string* out, size_t max = 0);
	virtual int pop_end(string* out, size_t max = 0);
	virtual void clear();

private:
	// �ڴ�ع��������ʺϹ�������С�ڴ�
	dbuf_guard* dbuf_;
	dbuf_guard* dbuf_internal_;

	// ��Ӧ�� acl ���е� ACL_JSON ����
	ACL_JSON *json_;
	// json �������еĸ��ڵ����
	json_node* root_;
	// ��ʱ�� json �ڵ��ѯ�����
	std::vector<json_node*> nodes_query_;
	// ������
	string* buf_;
	ACL_ITER* iter_;
};

} // namespace acl