#pragma once
#include "../acl_cpp_define.hpp"
#include <vector>
#include <string>
#include "../connpool/connect_client.hpp"
#include "http_header.hpp"

namespace acl {

class http_client;
class http_pipe;
class socket_stream;
class charset_conv;
class sslbase_conf;
class xml;
class json;

/**
 * HTTP �ͻ��������࣬�������֧�ֳ����ӣ�ͬʱ�����Ӷ�ʱ���Զ�����
 */
class ACL_CPP_API http_request : public connect_client {
public:
	/**
	 * ���캯����ͨ���ù��캯������� socket_stream ������
	 * ���ᱻ�رգ���Ҫ�������Լ��ر�
	 * @param client {socket_stream*} HTTP ���������󣬿���������˵�����
	 *  Ҳ��������Ӧ�˵�����������������ʱ��client �����Ƿ�ᱻ�Զ����٣�
	 *  ȡ���ڲ��� stream_fixed ��ֵ
	 * @param conn_timeout {int} �����������رգ����ڲ���
	 *  �Զ����ԣ���ʱ��Ҫ��ֵ��ʾ���ӷ������ĳ�ʱʱ��(��)��
	 *  ������������ IO ��д��ʱʱ���Ǵ� ��������м̳е�
	 * @param unzip {bool} �Ƿ�Է�������Ӧ�������Զ����н�ѹ
	 * ע��������ʵ�������ʹ��ʱ���û�Ӧ����ÿ�ε���ǰ����
	 * request_header::http_header::reset()
	 * @param stream_fixed {bool} ����ֵΪ true ʱ���� http_client ����
	 *  ������ʱ������� client �����󲻻ᱻ���٣���Ӧ���������٣������
	 *  ֵΪ false ʱ���򵱱���������ʱ��client ������Ҳ��������
	 */
	explicit http_request(socket_stream* client, int conn_timeout = 60,
		bool unzip = true, bool stream_fixed = true);

	/**
	 * ���캯�����ù��캯���ڲ������� socket_stream �������йر�
	 * @param addr {const char*} WEB ��������ַ����ַ��ʽ��domain:port��
	 *  �磺www.baidu.com:80, ���� http://www.baidu.com, �� www.baidu.com
	 * @param conn_timeout {int} Զ�����ӷ�������ʱʱ��(��)
	 * @param rw_timeout {int} IO ��д��ʱʱ��(��)
	 * @param unzip {bool} �Ƿ�Է�������Ӧ�������Զ����н�ѹ
	 */
	explicit http_request(const char* addr, int conn_timeout = 60,
		int rw_timeout = 60, bool unzip = true);

	virtual ~http_request();

	/**
	 * �����ڶ�ȡ������Ӧ����ʱ�Ƿ����ѹ�����ݽ��н�ѹ
	 * @param on {bool}
	 * @return {http_request&}
	 */
	http_request& set_unzip(bool on);

	/**
	 * ���ÿͻ��� SSL ͨ�ŷ�ʽ���ڲ�ȱʡΪ�� SSL ͨ�ŷ�ʽ
	 * @param conf {sslbase_conf*} �ͻ��� SSL ���ö���
	 * @return {http_request&}
	 */
	http_request& set_ssl(sslbase_conf* conf);

	/**
	 * ���� SSL ����ʱ�� SNI �ֶΣ����ô� API ���� SSL SNI �ֶκ��ڵ��ñ����
	 * reset() API ʱ�������Զ������� API ���õ�ֵ�������Ҫ����������Ҫ���µ���
	 * ������������Ӧ�����ÿգ�����Ӧ�ֶ��Զ����
	 * @param sni {const char*} ���Ϊ NULL�����Զ���ʹ�� HTTP ͷ�е� Host �ֶ�
	 * @return {http_request&}
	 */
	http_request& set_ssl_sni(const char* sni);

	/**
	 * ���� SSL ����ʱ�� SNI �ֶ�ǰ׺�����ô� API ���� SSL SNI �ֶ�ǰ׺��
	 * �ڵ��ñ���� reset() API ʱ�������Զ������� API ���õ�ֵ�������Ҫ������
	 * ����Ҫ���µ��ò����ÿ�ֵ
	 * @param prefix {const char*} SNI ǰ׺
	 * @return {http_request&}
	 */
	http_request& set_ssl_sni_prefix(const char* prefix);

	/**
	 * ���� SSL ����ʱ�� SNI �ֶκ�׺�����ô� API ���� SSL SNI �ֶ�ǰ׺��
	 * �ڵ��ñ���� reset() API ʱ�������Զ������� API ���õ�ֵ�������Ҫ������
	 * ����Ҫ���µ��ò����ÿ�ֵ
	 * @param suffix {const char*} SNI ��׺
	 * @return {http_request&}
	 */
	http_request& set_ssl_sni_suffix(const char* prefix);

	/**
	 * ��� HTTP ����ͷ����Ȼ���ڷ��ص� HTTP ����ͷ����������
	 * �Լ�������ͷ�ֶλ� http_header::reset()��������ͷ״̬��
	 * �ο���http_header ��
	 * @return {http_header&}
	 */
	http_header& request_header();

	/**
	 * ���ñ����ַ������������ַ����ǿ�ʱ����߽������ݱ߽����ַ���ת��
	 * @param local_charset {const char*} �����ַ���
	 * @return {http_header&}
	 */
	http_request& set_local_charset(const char* local_charset);

	/**
	 * ����HTTP GET������������
	 * @return {bool}
	 */
	bool get();

	/**
	 * ����HTTP POST������������
	 * @param data {const char*} �����������
	 * @param len {size_t} ���������峤��
	 * @return {bool}
	 */
	bool post(const char* data, size_t len);

	/**
	 * �� HTTP ���������� HTTP ����ͷ�� HTTP �����壬ͬʱ��
	 * HTTP ��������ȡ HTTP ��Ӧͷ�����ڳ����ӣ��������ж�ʱ
	 * ��������һ�Σ��ڵ�������ļ��� get_body ����ǰ������
	 * ���ñ�����(����� write_head/write_body)��
	 * ��������£��ú����ڷ������������ݺ��� HTTP ��Ӧͷ��
	 * �����û��ڱ��������� true ����Ե��ã�get_body() ��
	 * http_request::get_clinet()->read_body(char*, size_t)
	 * ������ HTTP ��Ӧ��������
	 * @param data {const void*} ���͵��������ַ���ǿ�ʱ�Զ���
	 *  POST �������ͣ����� GET ��������
	 * @param len {size_} data �ǿ�ʱָ�� data ���ݳ���
	 * @return {bool} �����������ݼ��� HTTP ��Ӧͷ�����Ƿ�ɹ�
	 */
	bool request(const void* data, size_t len);

	/////////////////////////////////////////////////////////////////////

	/**
	 * ��������ʽд����ʱ����Ҫ���ȵ��ñ��������� HTTP ����ͷ
	 * @return {bool} �Ƿ�ɹ�������ɹ��ſ��Լ������� write_body
	 */
	bool write_head();

	/**
	 * ��������ʽд����ʱ���ڵ��� write_head �󣬿���ѭ�����ñ�����
	 * ���� HTTP ���������ݣ����������������Ϊ��ֵʱ���ʾ����д�ꣻ
	 * �����������ݺ󣬸ú����ڲ����Զ���ȡ HTTP ��Ӧͷ���ݣ��û���
	 * �������� get_body/read_body ��ȡ HTTP ��Ӧ������
	 * @param data {const void*} ���ݵ�ַָ�룬����ֵΪ��ָ��ʱ��ʾ
	 *  ���ݷ������
	 * @param len {size_t} data �ǿ�ָ��ʱ��ʾ���ݳ���
	 * @return {bool} �����������Ƿ�ɹ�
	 *  ע����Ӧ�÷��������ݺ󣬱����ٵ���һ�α�������ͬʱ����������������
	 */
	bool write_body(const void* data, size_t len);

	/////////////////////////////////////////////////////////////////////

	/**
	 * ���������������ݺ��ڲ����Զ����ö� HTTP ��Ӧͷ���̣�����ͨ���˺���
	 * ��÷������Ӧ�� HTTP ״̬��(2xx, 3xx, 4xx, 5xx)��
	 * ��ʵ�ú����ڲ�ֻ�ǵ����� http_client::response_status ����
	 * @return {int}
	 */
	int http_status() const;

	/**
	 * ��� HTTP ��Ӧ�������峤��
	 * @return {int64) ����ֵ��Ϊ -1 ����� HTTP ͷ�����ڻ�û�г����ֶ�
	 */
#if defined(_WIN32) || defined(_WIN64)
	__int64 body_length(void) const;
#else
	long long int body_length() const;
#endif
	/**
	 * HTTP ������(��Ӧ���Ƿ��������ֳ�����)
	 * @return {bool}
	 */
	bool keep_alive() const;

	/**
	 * ��� HTTP ��Ӧͷ��ĳ���ֶ������ֶ�ֵ
	 * @param name {const char*} �ֶ���
	 * @return {const char*} �ֶ�ֵ��Ϊ��ʱ��ʾ������
	 */
	const char* header_value(const char* name) const;

	/**
	 * �Ƿ������������
	 * @return {bool}
	 */
	bool body_finish() const;

	/**
	 * ������ request �ɹ�����ñ���������ȡ��������Ӧ������
	 * ��������洢�ڹ涨�� xml ������
	 * @param out {xml&} HTTP ��Ӧ�����ݴ洢�ڸ� xml ������
	 * @param to_charset {const char*} ������ǿգ��ڲ��Զ�
	 *  ������ת�ɸ��ַ����洢�� xml ������
	 * @return {bool} �������Ƿ�ɹ�
	 * ע������Ӧ�������ر��ʱ��Ӧ�ô˺����������ڴ�Ĺ�
	 */
	bool get_body(xml& out, const char* to_charset = NULL);

	/**
	 * ������ request �ɹ�����ñ���������ȡ��������Ӧ������
	 * ��������洢�ڹ涨�� json ������
	 * @param out {json&} HTTP ��Ӧ�����ݴ洢�ڸ� json ������
	 * @param to_charset {const char*} ������ǿգ��ڲ��Զ�
	 *  ������ת�ɸ��ַ����洢�� json ������
	 * @return {bool} �������Ƿ�ɹ�
	 * ע������Ӧ�������ر��ʱ��Ӧ�ô˺����������ڴ�Ĺ�
	 */
	bool get_body(json& out, const char* to_charset = NULL);

	/**
	 * ������ request �ɹ�����ñ���������ȡ������ȫ����Ӧ����
	 * �洢������Ļ�������
	 * @param out {string&} �洢��Ӧ������
	 * @param to_charset {const char*} ������ǿգ��ڲ��Զ�
	 *  ������ת�ɸ��ַ����洢�� out ������
	 * ע������Ӧ�������ر��ʱ��Ӧ�ô˺����������ڴ�Ĺ�
	 */
	bool get_body(string& out, const char* to_charset = NULL);

	/**
	 * ������ request �ɹ�����ñ���������ȡ��������Ӧ���ݲ�
	 * �洢������Ļ������У�����ѭ�����ñ�������ֱ�����ݶ����ˣ�
	 * @param buf {char*} �洢������Ӧ������
	 * @param size {size_t} buf ��������С
	 * @return {int} ����ֵ == 0 ��ʾ��������ϣ�< 0 ��ʾ������
	 *  �ر����ӣ�> 0 ��ʾ�Ѿ����������ݣ��û�Ӧ��һֱ������ֱ��
	 *  ����ֵ <= 0 Ϊֹ
	 *  ע���ú�����������ԭʼ HTTP ���������ݣ�������ѹ���ַ���
	 *  ���룬�û��Լ�������Ҫ���д���
	 */
	int read_body(char* buf, size_t size);

	/**
	 * ������ request �ɹ�����ñ������� HTTP ��Ӧ�����壬����ѭ������
	 * ���������������ڲ��Զ���ѹ�����ݽ��н�ѹ������ڵ��ñ�����֮ǰ����
	 * set_charset �����˱����ַ�������ͬʱ�����ݽ����ַ���ת�����
	 * @param out {string&} �洢�������
	 * @param clean {bool} ÿ�ε��ñ�����ʱ���Ƿ�Ҫ�����Զ��������� out
	 *  ���������
	 * @param real_size {int*} ����ָ��ǿ�ʱ���洢��ѹǰ��������������
	 *  ���ȣ�����ڹ��캯����ָ���˷��Զ���ѹģʽ�Ҷ��������� > 0�����
	 *  ֵ�洢�ĳ���ֵӦ���뱾��������ֵ��ͬ������������δ�����κ�����ʱ��
	 *  ��ֵ�洢�ĳ���ֵΪ 0
	 * @return {int} == 0 ��ʾ����ϣ��������Ӳ�δ�رգ�>0 ��ʾ���ζ�����
	 *  ���������ݳ���(��Ϊ��ѹ�������ʱ�����ʾΪ��ѹ֮������ݳ��ȣ�
	 *  ����ʵ���������ݲ�ͬ����ʵ���������ݳ���Ӧ��ͨ������ real_size ��
	 *  ���); < 0 ��ʾ�������رգ���ʱ�� real_size �ǿգ��� real_size ��
	 *  ����ֵӦ��Ϊ 0
	 *  ������ 0 ʱ���ɵ��� body_finish �����ж��Ƿ����������������
	 */
	int read_body(string& out, bool clean = false, int* real_size = NULL);

	/**
	 * ���� request �ɹ�����ñ��������� HTTP ����˶�һ�����ݣ���ѭ������
	 * ��������ֱ������ false �� body_finish() ���� true Ϊֹ��
	 * �ڲ��Զ���ѹ�����ݽ�ѹ������ڵ��ñ�����֮ǰ���� set_charset ������
	 * �����ַ�������ͬʱ�����ݽ����ַ���ת�����
	 * @param out {string&} �洢�������
	 * @param nonl {bool} ������һ�������Ƿ��Զ�ȥ��β���� "\r\n" �� "\n"
	 * @param size {size_t*} ��ָ��ǿ�ʱ��Ŷ��������ݳ���
	 * @return {bool} �Ƿ����һ�����ݣ����� true ʱ��ʾ������һ�����ݣ�
	 *  ����ͨ�� body_finish() �Ƿ�Ϊ true ���ж��Ƿ���������Ѿ�������
	 *  ������һ������ �� nonl = true ʱ���� *size = 0�������� false ʱ
	 *  ��ʾδ���������Ҷ���ϣ�
	 *  *size �д���Ŷ��������ݳ���
	 */
	bool body_gets(string& out, bool nonl = true, size_t* size = NULL);

	/**
	 * ��ͨ�� http_request::request_header().set_range() ������
	 * range ������ʱ���˺��������������ص������Ƿ�֧�� range
	 * @return {bool}
	 */
	bool support_range() const;

#if defined(_WIN32) || defined(_WIN64)
	/**
	 * �������� http_request::request_header().set_range() �Ҷ�ȡ������
	 * ���ص�����ͷ�󣬴˺����������֧�ֶַι��ܵ���ʼƫ��λ��
	 * @return {acl_int64} ����������֧�� range ��ʽ���򷵻�ֵ < 0
	 */
	__int64 get_range_from(void) const;

	/**
	 * �������� http_request::request_header().set_range() �Ҷ�ȡ������
	 * ���ص�����ͷ�󣬴˺����������֧�ֶַι��ܽ���ƫ��λ��
	 * @return {acl_int64} ����������֧�� range ��ʽ���򷵻�ֵ < 0
	 */
	__int64 get_range_to(void) const;

	/**
	 * �������� http_request::request_header().set_range() �Ҷ�ȡ������
	 * ���ص�����ͷ�󣬴˺����������֧�ֶַι��ܵ������������С����ֵ
	 * ������ HTTP ��Ӧ�������С
	 * @return {acl_int64} ����������֧�� range ��ʽ���򷵻�ֵ < 0
	 */
	__int64 get_range_max(void) const;
#else
	long long int get_range_from() const;
	long long int get_range_to() const;
	long long int get_range_max() const;
#endif

	/**
	 * ��÷��������ص� Set-Cookie �ļ���
	 * @return {const std::vector<HttpCookie*>*} ���ؿձ�ʾ
	 *  û�� cookie �����������Ϊ��
	 */
	const std::vector<HttpCookie*>* get_cookies() const;

	/**
	 * ��÷��������ص� Set-Cookie ���õ�ĳ�� cookie ����
	 * @param name {const char*} cookie ��
	 * @param case_insensitive {bool} �Ƿ����ִ�Сд��true ��ʾ
	 *  �����ִ�Сд
	 * @return {const HttpCookie*} ���� NULL ��ʾ������
	 */
	const HttpCookie* get_cookie(const char* name,
		bool case_insensitive = true) const;

	/////////////////////////////////////////////////////////////////////

	/**
	 * ��� http_client HTTP ������������ͨ�����صĶ�����
	 * ��������Ӧ��ͷ�������ݣ��ο���http_client ��
	 * @return {http_client*} �����ؿ�ʱ��ʾ��������
	 */
	http_client* get_client() const;

	/**
	 * ��������״̬����ͬһ�����ӵĶ������ʱ����ô˺���
	 */
	void reset();

protected:
	/**
	 * ���� connect_client ���麯������ʽ���ñ����������������˵�����
	 * @return {bool} �����Ƿ�ɹ�
	 */
	virtual bool open();

private:
	char addr_[128];
	bool unzip_;
	sslbase_conf* ssl_conf_;
	std::string sni_host_;
	std::string sni_prefix_;
	std::string sni_suffix_;
	char local_charset_[64];
	charset_conv* conv_;
        http_client* client_;
	http_header  header_;
	bool cookie_inited_;
	std::vector<HttpCookie*>* cookies_;
#if defined(_WIN32) || defined(_WIN64)
	__int64 range_from_;
	__int64 range_to_;
	__int64 range_max_;
#else
	long long int range_from_;
	long long int range_to_;
	long long int range_max_;
#endif
	// ��д HTTP ����������ʱ���ñ�־λ��ʶ�Ƿ��������Թ�
	bool need_retry_;

	bool send_request(const void* data, size_t len);
	bool try_open(bool* reuse_conn);
	void close();
	void create_cookies();
	http_pipe* get_pipe(const char* to_charset);
	void set_charset_conv();
	void check_range();
};

} // namespace acl