#include <libidmefv2/idmefv2.h>
#include <string.h>

int main(){
    FILE *file;
    int ret;
    libidmefv2_io_t *pio;
    idmefv2_message_t *idmef, *idmefv2_heartbeat;
    idmefv2_alert_t *alert;
    idmefv2_heartbeat_t *heartbeat;
    idmefv2_classification_t *class;
    libidmefv2_string_t *str;
    idmefv2_time_t * time;

    file = fopen("/tmp/example.idmefv2", "a");

    libidmefv2_io_new(&pio);
    libidmefv2_io_set_file_io(pio, file);

    ret = idmefv2_message_new(&idmef);
    if ( ret < 0 ) {
            libidmefv2_perror(ret, "unable to create IDMEF message");
            return -1;
    }
    ret = idmefv2_message_new_alert(idmef, &alert);
    if ( ret < 0 ) {
            libidmefv2_perror(ret, "unable to create IDMEF alert");
            idmefv2_message_destroy(idmef);
            return -1;
    }

    ret = idmefv2_message_new(&idmefv2_heartbeat);
    if ( ret < 0 ) {
            libidmefv2_perror(ret, "unable to create IDMEF message");
            return -1;
    }
    ret = idmefv2_message_new_heartbeat(idmefv2_heartbeat, &heartbeat);
    if ( ret < 0 ) {
            libidmefv2_perror(ret, "unable to create IDMEF heartbeat");
            idmefv2_message_destroy(idmefv2_heartbeat);
            return -1;
    }

    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.messageid", "superid");
    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.heartbeat_interval", "66666");
    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.analyzer(0).analyzerid", "example.idmef");
    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.analyzer(0).name", "example.idmef");
    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.analyzer(0).manufacturer", "example.idmef");
    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.analyzer(0).model", "example.idmef");
    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.analyzer(0).version", "example.idmef");
    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.analyzer(0).class", "example.idmef");
    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.analyzer(0).ostype", "example.idmef");
    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.analyzer(0).osversion", "example.idmef");
    idmefv2_message_set_string(idmefv2_heartbeat, "heartbeat.analyzer(0).analyzeraction", "create");



    idmefv2_message_set_string(idmef, "alert.messageid", "superid");
    idmefv2_message_set_string(idmef, "alert.analyzerhash", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.classification.ident", "OSVDB-XXXX");
    idmefv2_message_set_string(idmef, "alert.classification.text", "OSVDB-XXXX");
    idmefv2_message_set_string(idmef, "alert.classification.reference(0).name", "OSVDB-XXXX");
    idmefv2_message_set_string(idmef, "alert.classification.reference(0).origin", "osvdb");
    idmefv2_message_set_string(idmef, "alert.classification.reference(0).url", "http://my.url/");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(0).address(0).address", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(0).address(0).category", "ipv4-addr");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).analyzerid", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).name", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).manufacturer", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).model", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).version", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).class", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).ostype", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).osversion", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).analyzeraction", "create");

    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.nodename(0).category", "dns");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.location.name", "example.idmef");

    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(0).address(0).ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(0).address(0).category", "ipv4-addr");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(0).address(0).vlan_name", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(0).address(0).vlan_num", "5");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(0).address(0).address", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(0).address(0).netmask", "example.idmef");

    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(1).address(1).ident", "ezf");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(1).address(1).category", "ipv4-addr");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(1).address(1).vlan_name", "ezffez");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(1).address(1).vlan_num", "12");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(1).address(1).address", "zefez");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).node.interface(1).address(1).netmask", "zefez");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).process.ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).process.name", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).process.pid", "666");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).process.path", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).process.arg(0)", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).process.arg(1)", "gumi");
    idmefv2_message_set_string(idmef, "alert.analyzer(0).process.env(0)", "example.idmef");


    idmefv2_message_set_string(idmef, "alert.source(0).ident", "source style");
    idmefv2_message_set_string(idmef, "alert.source(0).spoofed", "yes");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(0).name", "eth5");
    idmefv2_message_set_string(idmef, "alert.source(0).node.ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.source(0).node.nodename(0).category", "dns");

    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(0).address(0).ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(0).address(0).category", "ipv4-addr");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(0).address(0).vlan_name", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(0).address(0).vlan_num", "5");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(0).address(0).address", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(0).address(0).netmask", "example.idmef");

    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(1).address(1).ident", "ezf");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(1).address(1).category", "ipv4-addr");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(1).address(1).vlan_name", "ezffez");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(1).address(1).vlan_num", "12");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(1).address(1).address", "zefez");
    idmefv2_message_set_string(idmef, "alert.source(0).node.interface(1).address(1).netmask", "zefez");
    idmefv2_message_set_string(idmef, "alert.source(0).process.ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.source(0).process.name", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.source(0).process.pid", "666");
    idmefv2_message_set_string(idmef, "alert.source(0).process.path", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.source(0).process.arg(0)", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.source(0).process.arg(1)", "gumi");
    idmefv2_message_set_string(idmef, "alert.source(0).process.env(0)", "example.idmef");

    idmefv2_message_set_string(idmef, "alert.source(0).service.ident", "example");
    idmefv2_message_set_string(idmef, "alert.source(0).service.iana_protocol_number", "example");
    idmefv2_message_set_string(idmef, "alert.source(0).service.iana_protocol_name", "6");
    idmefv2_message_set_string(idmef, "alert.source(0).service.name", "example");
    idmefv2_message_set_string(idmef, "alert.source(0).service.port", "6");
    idmefv2_message_set_string(idmef, "alert.source(0).service.portlist", "example");
    idmefv2_message_set_string(idmef, "alert.source(0).service.protocol", "example");

    idmefv2_message_set_string(idmef, "alert.target(0).ident", "target style");
    idmefv2_message_set_string(idmef, "alert.target(0).decoy", "yes");
    idmefv2_message_set_string(idmef, "alert.target(0).interface", "eth2");
    idmefv2_message_set_string(idmef, "alert.target(0).node.ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).node.nodename(0).category", "dns");
    idmefv2_message_set_string(idmef, "alert.target(0).node.location.name", "example.idmef");

    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(0).address(0).ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(0).address(0).category", "ipv4-addr");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(0).address(0).vlan_name", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(0).address(0).vlan_num", "5");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(0).address(0).address", "192.168.2.50");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(0).address(0).netmask", "example.idmef");

    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(1).address(1).ident", "ezf");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(1).address(1).category", "ipv4-addr");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(1).address(1).vlan_name", "ezffez");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(1).address(1).vlan_num", "12");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(1).address(1).address", "zefez");
    idmefv2_message_set_string(idmef, "alert.target(0).node.interface(1).address(1).netmask", "zefez");
    idmefv2_message_set_string(idmef, "alert.target(0).process.ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).process.name", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).process.pid", "666");
    idmefv2_message_set_string(idmef, "alert.target(0).process.path", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).process.arg(0)", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).process.arg(1)", "gumi");
    idmefv2_message_set_string(idmef, "alert.target(0).process.env(0)", "example.idmef");


    idmefv2_message_set_string(idmef, "alert.target(0).service.ident", "example");
    idmefv2_message_set_string(idmef, "alert.target(0).service.iana_protocol_number", "example");
    idmefv2_message_set_string(idmef, "alert.target(0).service.iana_protocol_name", "6");
    idmefv2_message_set_string(idmef, "alert.target(0).service.name", "example");
    idmefv2_message_set_string(idmef, "alert.target(0).service.port", "6");
    idmefv2_message_set_string(idmef, "alert.target(0).service.portlist", "example");
    idmefv2_message_set_string(idmef, "alert.target(0).service.protocol", "example");

    idmefv2_message_set_string(idmef, "alert.target(0).file(0).ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).name", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).path", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).data_size", "6");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).disk_size", "6");

    idmefv2_message_set_string(idmef, "alert.target(0).file(0).file_access(0).permission(0)", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).file_access(0).permission(1)", "example.idmef");

    idmefv2_message_set_string(idmef, "alert.target(0).file(0).file_access(0).user_id(0).ident", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).file_access(0).user_id(0).type", "target-user");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).file_access(0).user_id(0).tty", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).file_access(0).user_id(0).name", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).file_access(0).user_id(0).number", "6");

    idmefv2_message_set_string(idmef, "alert.target(0).file(0).linkage(0).category", "stream");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).linkage(0).name", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).linkage(0).path", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).linkage(0).file.name", "example.idmef dans linkage");

    idmefv2_message_set_string(idmef, "alert.target(0).file(0).inode.number", "6");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).inode.major_device", "6");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).inode.minor_device", "6");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).inode.c_major_device", "6");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).inode.c_minor_device", "6");

    idmefv2_message_set_string(idmef, "alert.target(0).file(0).checksum.value", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).checksum.key", "example.idmef");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).checksum.algorithm", "md5");

    idmefv2_message_set_string(idmef, "alert.target(0).file(0).category", "current");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).fstype", "ufs");
    idmefv2_message_set_string(idmef, "alert.target(0).file(0).file_type", "example.idmef");


    idmefv2_message_set_string(idmef, "alert.assessment.impact.severity", "high");
    idmefv2_message_set_string(idmef, "alert.assessment.impact.completion", "failed");
    idmefv2_message_set_string(idmef, "alert.assessment.impact.impacttype(0).name", "dos");
    idmefv2_message_set_string(idmef, "alert.assessment.impact.description", "example.idmef");

    idmefv2_message_set_string(idmef, "alert.additional_data(0).meaning", "additional1");
    idmefv2_message_set_string(idmef, "alert.additional_data(0).data", "value de l'additional data 1");

    idmefv2_message_set_string(idmef, "alert.additional_data(1).meaning", "additional2");
    idmefv2_message_set_string(idmef, "alert.additional_data(1).data", "value de l'additional data 2");

    idmefv2_time_new_from_gettimeofday(&time);
    idmefv2_alert_set_detect_time(alert, time);

    idmefv2_message_new_version(idmefv2_heartbeat, &str);
    libidmefv2_string_set_constant(str, "2.0");

    idmefv2_message_new_version(idmef, &str);
    libidmefv2_string_set_constant(str, "2.0");

    idmefv2_message_print(idmef, pio);
    idmefv2_message_write(idmef, pio);
}
