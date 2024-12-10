#pragma once
#include "../acl_cpp_define.hpp"
#if defined(_WIN32) || defined(_WIN64)
#include <WinSock2.h>
#endif
#include <string>
#include "istream.hpp"
#include "ostream.hpp"

struct ACL_VSTREAM;

namespace acl {

class ACL_CPP_API socket_stream : public istream , public ostream {
public:
	socket_stream();
	~socket_stream();

	/**
	 * �����׽��ִ򿪵�һ��������
	 * @param fd �׽���
	 * @param udp_mode {bool} �Ƿ��� UDP ��ʽ
	 * @return {bool} �����Ƿ�ɹ�
	 */
#if defined(_WIN32) || defined(_WIN64)
	bool open(SOCKET fd, bool udp_mode = false);
#else
	bool open(int fd, bool udp_mode = false);
#endif

	/**
	 * ���� ACL_VSTREAM ����������
	 * @param vstream {ACL_VSTREAM*}
	 * @param udp_mode {bool} �Ƿ��� UDP ��ʽ
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool open(ACL_VSTREAM* vstream, bool udp_mode = false);

	/**
	 * ����Զ�̷�������������������
	 * @param addr {const char*} ��������ַ, ���������׽ӿڷ�����(UNIXƽ̨),
	 *  ���׽ӵ�ַ��/tmp/test.sock����Linux ƽ̨�»������ӳ������׽��֣���
	 *  abastract unix socket��Ϊ������ͨ�����ļ�·����unix���׽ӵ�ַ����
	 *  �� acl ���й涨�����ַ��һ���ֽ�Ϊ @������Ϊ�� Linux �������׽���
	 *  ��abstract unix domain socket��������ע��ù��ܽ��� Linux ƽ̨֧��,
	 *  �������磺@/tmp/test.sock��;
	 *  remote_addr[@local_ip]|[#interface], ��:
	 *  1. www.sina.com|80@60.28.250.199, ��˼�ǰ󶨱�������ַΪ:
	 *     60.28.250.199, Զ������ www.sina.com �� 80 �˿�;
	 *  2.  211.150.111.12|80@192.168.1.1����ʾ���󶨱��ص�ַ��
	 *  3. 211.150.111.12|80#eth1����ʾ���󶨱���ָ������;
	 *  4. �����OS �Զ��󶨱��� IP ��ַ�������дΪ��www.sina.com:80;
	 * @param conn_timeout {int} ���ӳ�ʱʱ��(��λֵȡ���� use_ms)
	 * @param rw_timeout {int} ��д��ʱʱ��(��λֵȡ���� use_ms)
	 * @param unit {time_unit_t} ��ʱʱ���ʱ�䵥λ
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool open(const char* addr, int conn_timeout, int rw_timeout,
		time_unit_t unit = time_unit_s);

	/**
	 * �󶨱��� UDP ��ַ������ UDP ͨ�Ŷ���
	 * @param addr {const char*} ������ַ����ʽ��ip:port���õ�ַҲ����Ϊ
	 *  UNIX ���׽��ֻ� Linux �������׽��֣�Linux abstract unix socket��
	 * @param rw_timeout {int} ��д��ʱʱ��(��)
	 * @param flags {unsigned} �ñ�־λ�Ķ���μ� server_socket.hpp
	 * @return {bool} ���Ƿ�ɹ�
	 */
	bool bind_udp(const char* addr, int rw_timeout = -1, unsigned flags = 0);

	/**
	 * �󶨲������鲥�׽ӿ�ͨ�Ŷ���
	 * @param addr {const char*} �鲥 IP ��ַ
	 * @param iface {const char*} ���������շ����ݰ��� IP ��ַ
	 * @param port {int} �鲥�˿ں�
	 * @param rw_timeout {int} IO ��д��ʱʱ��
	 * @param flags {unsigned} �ñ�־λ�Ķ���μ� server_socket.hpp
	 * @return {bool} ���Ƿ�ɹ�
	 */
	bool bind_multicast(const char* addr, const char* iface, int port,
		int rw_timeout = -1, unsigned flags = 0);

	/**
	 * �� bind_multicast �ɹ��󣬿��Ե��ñ��������ù㲥���� TTL ֵ
	 * @param ttl {int} ��ֵ�ķ�ΧΪ 1--255
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool multicast_set_ttl(int ttl);

	/**
	 * �� bind_multicast �ɹ��󣬿��Ե��ñ��������ù㲥���ı��� IP ��ַ
	 * @param iface {const char*}
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool multicast_set_if(const char* iface);

	/**
	 * �� bind_multicast �ɹ��󣬿��Ե��ñ����������뿪�鲥
	 * @param addr {const char*} �㲥 IP
	 * @param iface {const char*} ���� IP
	 * @return {bool} �Ƿ�ɹ�
	 */
	bool multicast_drop(const char *addr, const char *iface);

	/**
	 * �ر��׽ӿڶ�����
	 * @return {bool}
	 */
	bool shutdown_read();

	/**
	 * �ر��׽ӿ�д����
	 * @return {bool}
	 */
	bool shutdown_write();

	/**
	 * �ر��׽ӿڶ�д����
	 * @return {bool}
	 */
	bool shutdown_readwrite();

	/**
	 * ����������������׽������Ӿ��
	 * @return {ACL_SOCKET} ���������򷵻� - 1(UNIX ƽ̨)
	 *  �� INVALID_SOCKET(win32ƽ̨)
	 */
#if defined(_WIN32) || defined(_WIN64)
	SOCKET sock_handle(void) const;
#else
	int   sock_handle() const;
#endif

	/**
	 * ����׽�����������İ󶨹�ϵ��ͬʱ���׽��ַ��ظ��û�����
	 * �����׽��ֵĹ���Ȩ�����û��������������ͷ�ʱ����رո���
	 * ���֣����û��ӹܸ��׽��ֺ��������뽫��ر�
	 * ������� close/open �ĵ����������⣬�����ĵ���(��������
	 * ���д����)��������
	 * @return {ACL_SOCKET} ���� ACL_SOCKET_INVALID ��ʾ��������
	 *  �Ѿ����׽��ֽ��
	 */
#if defined(_WIN32) || defined(_WIN64)
	SOCKET unbind_sock(void);
#else
	int    unbind_sock();
#endif

	/**
	 * ��� socket ������
	 * @return {int} ����ֵ�У�AF_INET, AF_INT6, AF_UNIX������ʱ���� -1
	 */
	int sock_type() const;

	/**
	 * ���Զ�����ӵĵ�ַ
	 * @param full {bool} �Ƿ���������ַ������IP:PORT������ò���
	 *  Ϊ false��������� IP�����򷵻� IP:PORT
	 * @return {const char*} Զ�����ӵ�ַ��������ֵ == '\0' ���ʾ
	 *  �޷����Զ�����ӵ�ַ
	 */
	const char* get_peer(bool full = false) const;

	/**
	 * ���Զ�����ӵ� IP ��ַ
	 * @return {const char*} Զ�����ӵ�ַ��������ֵ == '\0' ���ʾ
	 *  �޷����Զ�����ӵ�ַ
	 */
	const char* get_peer_ip() const;

	/**
	 * ����Զ�����Ӷ���ĵ�ַ������ TCP ���䷽ʽ������Ҫ��ʾ���ô˺���
	 * ����Զ�̶����ַ��UDP ���䷽ʽʱ��Ҫ���ô˺�������Զ�̵�ַ��Ȼ��
	 * �ſ�����Զ������д����
	 * @param addr {const char*} Զ�����Ӷ���ĵ�ַ����ʽ��ip:port
	 * @return {bool} ��������δ��ʱ���� false
	 */
	bool set_peer(const char* addr);

	/**
	 * ������ӵı��ص�ַ
	 * @param full {bool} �Ƿ���������ַ������IP:PORT������ò���
	 *  Ϊ false��������� IP�����򷵻� IP:PORT
	 * @return {const char*} �����ӵı��ص�ַ��������ֵ == "" ���ʾ
	 *  �޷���ñ��ص�ַ
	 */
	const char* get_local(bool full = false) const;

	/**
	 * ������ӵı��� IP ��ַ
	 * @return {const char*} �����ӵı��ص�ַ��������ֵ == "" ���ʾ
	 *  �޷���ñ��ص�ַ
	 */
	const char* get_local_ip() const;

	/**
	 * ���ñ��ص�ַ
	 * @param addr {const char*} ��ַ����ʽ��ip:port
	 * @return {bool} ��������δ��ʱ���� false
	 */
	bool set_local(const char* addr);

	/**
	 * ����׽ӿ����ӵĴ��״̬(�ڲ�ʹ���˷��������ķ�ʽ����̽��)
	 * @param tc1 {double*} �ǿ�ʱ��¼��һ�׶κ�ʱ��ms)
	 * @param tc2 {double*} �ǿ�ʱ��¼�ڶ��׶κ�ʱ��ms)
	 * @return {bool} ����������δ�򿪻��Ѿ��ر�ʱ�ú������� false�����
	 *  ���������򷵻� true
	 */
	bool alive(double* tc1 = NULL, double* tc2 = NULL) const;

	/**
	 * ���� TCP �׽��ֵ� nodelay ����
	 * @param on {bool} true ��ʾ�򿪣�false ��ʾ�ر�
	 * @return {socket_stream&}
	 */
	socket_stream& set_tcp_nodelay(bool on);

	/**
	 * ���� TCP �׽��ֵ� SO_LINGER ѡ��
	 * @param on {bool} �Ƿ����� SO_LINGER ѡ��
	 * @param linger {int} ��SO_LINGER��ʱ��ȡ�� timed_wait ��ʱ�䣬��λΪ��
	 * @return {socket_stream&}
	 */
	socket_stream& set_tcp_solinger(bool on, int linger);

	/**
	 * ���� TCP �׽��ֵ�д��������С
	 * @param size {int} ���������ô�С
	 * @return {socket_stream&}
	 */
	socket_stream& set_tcp_sendbuf(int size);

	/**
	 * ���� TCP �׽��ֵĶ���������С
	 * @param size {int} ���������ô�С
	 * @return {socket_stream&}
	 */
	socket_stream& set_tcp_recvbuf(int size);

	/**
	 * ���� TCP �׽��ֵķ�����״̬
	 * @param on {bool} �Ƿ�����Ϊ������״̬����Ϊ true ʱ��
	 *  ����׽��ֱ���Ϊ������״̬������Ϊ����״̬
	 * @return {socket_stream&}
	 */
	socket_stream& set_tcp_non_blocking(bool on);

	/**
	 * ��� TCP �׽����Ƿ������� nodelay ѡ��
	 * @return {bool} true ��ʾ�򿪣�false ��ʾ�ر�
	 */
	bool get_tcp_nodelay() const;

	/**
	 * ��� TCP �׽��ֵ� linger ֵ
	 * @return {int} ���� -1 ��ʾδ���� linger ѡ����ڲ�������>= 0
	 *  ��ʾ������ linger ѡ���Ҹ�ֵ��ʾ�׽��ֹرպ�� TCP �������ں���
	 *  ά�� TIME_WAIT ״̬�Ķ���ʱ��(��)
	 */
	int get_tcp_solinger() const;

	/**
	 * ��ȡ TCP �׽��ֵ�д��������С
	 * @return {int} ��������С
	 */
	int get_tcp_sendbuf() const;

	/**
	 * ��ȡ TCP �׽��ֵĶ���������С
	 * @return {int} ��������С
	 */
	int get_tcp_recvbuf() const;

	/**
	 * �жϵ�ǰ�׽����Ƿ������˷�����ģʽ
	 * @return {bool}
	 * ע���÷���Ŀǰ��֧�� UNIX ƽ̨
	 */
	bool get_tcp_non_blocking() const;

private:
	std::string ipbuf_;
	const char* get_ip(const char* addr, std::string& out);
};

} // namespace acl