#include <stdio.h>
#include <stdlib.h>
static const unsigned int opmask = 63;
//OPCODE's 6 bits
static const unsigned int rmask = 31;
//each register's 5 bits
static const unsigned int funcmask = 63;
//FUNC's 6 bits
static const unsigned int immedmask = 32767;
//IMMEDIATE's chunk of 16 bits
static const unsigned int addrmask = 67108863;
//ADDRESS's chunk of 26 bits
static FILE * fin;
static FILE * fout;
static char type;
static int jcnt, icnt, rcnt = 0;
static int fwdcnt, bkwcnt, notcnt = 0;
static int scnt, lcnt = 0;
static int rw[32] = {0};
static int rr[32] = {0};

int
nottaken(unsigned int pc){
    int offset = ftell(fin);
    unsigned int nxtaddr;
    fscanf(fin, "%08x", &nxtaddr);
    fseek(fin, offset, SEEK_SET); 
    return nxtaddr == (pc + 4);
} 

int
fwddir(unsigned int pc, unsigned int addr){
    int offset = ftell(fin);
    unsigned int nxtaddr;
    fscanf(fin, "%08x", &nxtaddr);
    fseek(fin, offset, SEEK_SET);
    return nxtaddr > (pc + 4);
}

void
rstats(unsigned int instr, unsigned int funct){
    unsigned int rs, rt, rd;
    rs = (instr >> 21) & rmask;
    rt = (instr >> 16) & rmask;
    rd = (instr >> 11) & rmask;
    switch(funct){
        case 0x08: 
            rr[rs]++;
            break;
        case 0x00: 
            rr[rt]++;
            rw[rd]++;
            break;
        case 0x02:
            rr[rt]++;
            rw[rt]++;
            break;
        default:
            rr[rs]++;
            rr[rt]++;
            rw[rd]++;
            break;
    }
    rcnt++;
}

void 
jstats(unsigned int pc, unsigned int instr){
    unsigned int jaddr = (instr >> 5) & addrmask;
    if(fwddir(pc, jaddr)){
        fwdcnt++;
    }else{
        bkwcnt++;
    }
    jcnt++;
}

void 
istats(unsigned int opcode, unsigned int pc, unsigned int instr, unsigned int funct){
    icnt++;
    unsigned int jaddr;
    unsigned int rs, rt;
    rs = (instr >> 21) & rmask;
    rt = (instr >> 16) & rmask;
    if(funct == 0xf){
        rw[rt]++;
        return;
    }
    if(opcode == 4 || opcode == 5){
        rr[rs]++;
        rr[rt]++;
        jaddr = instr & immedmask;
        if(nottaken(pc)){
            notcnt++;
        }else if(fwddir(pc, jaddr)){
            fwdcnt++;
        }else{
            bkwcnt++;
        }
        return;
    }
    if(opcode == 0x28 || opcode == 0x29 || opcode == 0x2b){
        rr[rs]++;
        rr[rt]++;
        scnt++;
        return;
    }
    if(opcode == 0x23 || opcode == 0x24 || opcode == 0x25){
        lcnt++;
    }
    rr[rs]++;
    rw[rt]++;
}

void
decode(unsigned int pc, unsigned int instr){
    unsigned int opcode = (instr >> 26) & opmask;
    unsigned int funct = instr & funcmask;
        switch(opcode){
            case 0: rstats(instr, funct);
                break;
            case 2: jstats(pc, instr);
                break;
            case 3: jstats(pc, instr);
                    rw[31]++;
                break;
            default: istats(opcode, pc, instr, funct);
                break;
    }
}

void
printstats(FILE * fp){
    fprintf(fp, "insts: %d\n", jcnt + icnt + rcnt);
    fprintf(fp, "r-type: %d\n", rcnt);
    fprintf(fp, "i-type: %d\n", icnt);
    fprintf(fp, "j-type: %d\n", jcnt);
    fprintf(fp, "fwd-taken: %d\n", fwdcnt);
    fprintf(fp, "bkw-taken: %d\n", bkwcnt);
    fprintf(fp, "not-taken: %d\n", notcnt);
    fprintf(fp, "loads: %d\n", lcnt);
    fprintf(fp, "stores: %d\n", scnt);
    for(int i = 0; i < 32; i++){
        fprintf(fp, "reg-%d: %d %d\n", i, rr[i], rw[i]);
    }
}

int 
main(int argc, char * argv[]){
    fin = fopen("trace.txt", "r");
    unsigned int pc;
    unsigned int instr;
    while((fscanf(fin, "%08x", &pc) != EOF)){
        fscanf(fin, "%08x", &instr);
        decode(pc, instr);
    }
    fout = fopen("statistics.txt", "w+");
    printstats(fout);
    fclose(fin);
    fclose(fout); 
}
