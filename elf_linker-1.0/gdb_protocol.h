/*
ELF Loader - chargeur/implanteur d'ex�cutables au format ELF � but p�dagogique
Copyright (C) 2012 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
#ifndef __GDB_PROTOCOL_H__
#define __GDB_PROTOCOL_H__
#include <stdint.h>

typedef struct gdb_protocol_data *gdb_protocol_data_t;

gdb_protocol_data_t gdb_init_data(int fd);
void gdb_destroy_data(gdb_protocol_data_t gdb);
void gdb_write_register(gdb_protocol_data_t gdb, uint8_t num, uint32_t value);
void gdb_write_memory(gdb_protocol_data_t gdb, uint32_t address, void *buffer, size_t size);
void gdb_cont(gdb_protocol_data_t gdb);
int gdb_step(gdb_protocol_data_t gdb);
void gdb_require_retransmission(gdb_protocol_data_t gdb);
int gdb_packet_check(gdb_protocol_data_t gdb, char *packet, int length);
void gdb_transmit_packet(gdb_protocol_data_t gdb);

#endif
