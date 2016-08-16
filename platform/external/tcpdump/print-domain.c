/*
 * Copyright (c) 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static const char rcsid[] _U_ =
    "@(#) $Header: /tcpdump/master/tcpdump/print-domain.c,v 1.98 2007-12-09 01:40:32 guy Exp $ (LBL)";
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <tcpdump-stdinc.h>

#include "nameser.h"

#include <stdio.h>
#include <string.h>

#include "interface.h"
#include "addrtoname.h"
#include "extract.h"                    

static const char *ns_ops[] = {
	"", " inv_q", " stat", " op3", " notify", " update", " op6", " op7",
	" op8", " updataA", " updateD", " updateDA",
	" updateM", " updateMA", " zoneInit", " zoneRef",
};

static const char *ns_resp[] = {
	"", " FormErr", " ServFail", " NXDomain",
	" NotImp", " Refused", " YXDomain", " YXRRSet",
	" NXRRSet", " NotAuth", " NotZone", " Resp11",
	" Resp12", " Resp13", " Resp14", " NoChange",
};

static const u_char *
ns_nskip(register const u_char *cp)
{
	register u_char i;

	if (!TTEST2(*cp, 1))
		return (NULL);
	i = *cp++;
	while (i) {
		if ((i & INDIR_MASK) == INDIR_MASK)
			return (cp + 1);
		if ((i & INDIR_MASK) == EDNS0_MASK) {
			int bitlen, bytelen;

			if ((i & ~INDIR_MASK) != EDNS0_ELT_BITLABEL)
				return(NULL); 
			if (!TTEST2(*cp, 1))
				return (NULL);
			if ((bitlen = *cp++) == 0)
				bitlen = 256;
			bytelen = (bitlen + 7) / 8;
			cp += bytelen;
		} else
			cp += i;
		if (!TTEST2(*cp, 1))
			return (NULL);
		i = *cp++;
	}
	return (cp);
}

static const u_char *
blabel_print(const u_char *cp)
{
	int bitlen, slen, b;
	const u_char *bitp, *lim;
	char tc;

	if (!TTEST2(*cp, 1))
		return(NULL);
	if ((bitlen = *cp) == 0)
		bitlen = 256;
	slen = (bitlen + 3) / 4;
	lim = cp + 1 + slen;

	
	printf("\\[x");
	for (bitp = cp + 1, b = bitlen; bitp < lim && b > 7; b -= 8, bitp++) {
		TCHECK(*bitp);
		printf("%02x", *bitp);
	}
	if (b > 4) {
		TCHECK(*bitp);
		tc = *bitp++;
		printf("%02x", tc & (0xff << (8 - b)));
	} else if (b > 0) {
		TCHECK(*bitp);
		tc = *bitp++;
		printf("%1x", ((tc >> 4) & 0x0f) & (0x0f << (4 - b)));
	}
	printf("/%d]", bitlen);
	return lim;
trunc:
	printf(".../%d]", bitlen);
	return NULL;
}

static int
labellen(const u_char *cp)
{
	register u_int i;

	if (!TTEST2(*cp, 1))
		return(-1);
	i = *cp;
	if ((i & INDIR_MASK) == EDNS0_MASK) {
		int bitlen, elt;
		if ((elt = (i & ~INDIR_MASK)) != EDNS0_ELT_BITLABEL) {
			printf("<ELT %d>", elt);
			return(-1);
		}
		if (!TTEST2(*(cp + 1), 1))
			return(-1);
		if ((bitlen = *(cp + 1)) == 0)
			bitlen = 256;
		return(((bitlen + 7) / 8) + 1);
	} else
		return(i);
}

const u_char *
ns_nprint(register const u_char *cp, register const u_char *bp)
{
	register u_int i, l;
	register const u_char *rp = NULL;
	register int compress = 0;
	int chars_processed;
	int elt;
	int data_size = snapend - bp;

	if ((l = labellen(cp)) == (u_int)-1)
		return(NULL);
	if (!TTEST2(*cp, 1))
		return(NULL);
	chars_processed = 1;
	if (((i = *cp++) & INDIR_MASK) != INDIR_MASK) {
		compress = 0;
		rp = cp + l;
	}

	if (i != 0)
		while (i && cp < snapend) {
			if ((i & INDIR_MASK) == INDIR_MASK) {
				if (!compress) {
					rp = cp + 1;
					compress = 1;
				}
				if (!TTEST2(*cp, 1))
					return(NULL);
				cp = bp + (((i << 8) | *cp) & 0x3fff);
				if ((l = labellen(cp)) == (u_int)-1)
					return(NULL);
				if (!TTEST2(*cp, 1))
					return(NULL);
				i = *cp++;
				chars_processed++;

				if (chars_processed >= data_size) {
					printf("<LOOP>");
					return (NULL);
				}
				continue;
			}
			if ((i & INDIR_MASK) == EDNS0_MASK) {
				elt = (i & ~INDIR_MASK);
				switch(elt) {
				case EDNS0_ELT_BITLABEL:
					if (blabel_print(cp) == NULL)
						return (NULL);
					break;
				default:
					
					printf("<ELT %d>", elt);
					return(NULL);
				}
			} else {
				if (fn_printn(cp, l, snapend))
					return(NULL);
			}

			cp += l;
			chars_processed += l;
			putchar('.');
			if ((l = labellen(cp)) == (u_int)-1)
				return(NULL);
			if (!TTEST2(*cp, 1))
				return(NULL);
			i = *cp++;
			chars_processed++;
			if (!compress)
				rp += l + 1;
		}
	else
		putchar('.');
	return (rp);
}

static const u_char *
ns_cprint(register const u_char *cp)
{
	register u_int i;

	if (!TTEST2(*cp, 1))
		return (NULL);
	i = *cp++;
	if (fn_printn(cp, i, snapend))
		return (NULL);
	return (cp + i);
}

const struct tok ns_type2str[] = {
	{ T_A,		"A" },			
	{ T_NS,		"NS" },			
	{ T_MD,		"MD" },			
	{ T_MF,		"MF" },			
	{ T_CNAME,	"CNAME" },		
	{ T_SOA,	"SOA" },		
	{ T_MB,		"MB" },			
	{ T_MG,		"MG" },			
	{ T_MR,		"MR" },			
	{ T_NULL,	"NULL" },		
	{ T_WKS,	"WKS" },		
	{ T_PTR,	"PTR" },		
	{ T_HINFO,	"HINFO" },		
	{ T_MINFO,	"MINFO" },		
	{ T_MX,		"MX" },			
	{ T_TXT,	"TXT" },		
	{ T_RP,		"RP" },			
	{ T_AFSDB,	"AFSDB" },		
	{ T_X25,	"X25" },		
	{ T_ISDN,	"ISDN" },		
	{ T_RT,		"RT" },			
	{ T_NSAP,	"NSAP" },		
	{ T_NSAP_PTR,	"NSAP_PTR" },
	{ T_SIG,	"SIG" },		
	{ T_KEY,	"KEY" },		
	{ T_PX,		"PX" },			
	{ T_GPOS,	"GPOS" },		
	{ T_AAAA,	"AAAA" },		
	{ T_LOC,	"LOC" },		
	{ T_NXT,	"NXT" },		
	{ T_EID,	"EID" },		
	{ T_NIMLOC,	"NIMLOC" },		
	{ T_SRV,	"SRV" },		
	{ T_ATMA,	"ATMA" },		
	{ T_NAPTR,	"NAPTR" },		
	{ T_KX,		"KX" },			
	{ T_CERT,	"CERT" },		
	{ T_A6,		"A6" },			
	{ T_DNAME,	"DNAME" },		
	{ T_SINK, 	"SINK" },
	{ T_OPT,	"OPT" },		
	{ T_APL, 	"APL" },		
	{ T_DS,		"DS" },			
	{ T_SSHFP,	"SSHFP" },		
	{ T_IPSECKEY,	"IPSECKEY" },		
	{ T_RRSIG, 	"RRSIG" },		
	{ T_NSEC,	"NSEC" },		
	{ T_DNSKEY,	"DNSKEY" },		
	{ T_SPF,	"SPF" },		
	{ T_UINFO,	"UINFO" },
	{ T_UID,	"UID" },
	{ T_GID,	"GID" },
	{ T_UNSPEC,	"UNSPEC" },
	{ T_UNSPECA,	"UNSPECA" },
	{ T_TKEY,	"TKEY" },		
	{ T_TSIG,	"TSIG" },		
	{ T_IXFR,	"IXFR" },		
	{ T_AXFR,	"AXFR" },		
	{ T_MAILB,	"MAILB" },		
	{ T_MAILA,	"MAILA" },		
	{ T_ANY,	"ANY" },
	{ 0,		NULL }
};

const struct tok ns_class2str[] = {
	{ C_IN,		"IN" },		
	{ C_CHAOS,	"CHAOS" },
	{ C_HS,		"HS" },
	{ C_ANY,	"ANY" },
	{ 0,		NULL }
};

static const u_char *
ns_qprint(register const u_char *cp, register const u_char *bp, int is_mdns)
{
	register const u_char *np = cp;
	register u_int i, class;

	cp = ns_nskip(cp);

	if (cp == NULL || !TTEST2(*cp, 4))
		return(NULL);

	
	i = EXTRACT_16BITS(cp);
	cp += 2;
	printf(" %s", tok2str(ns_type2str, "Type%d", i));
	
	i = EXTRACT_16BITS(cp);
	cp += 2;
	if (is_mdns)
		class = (i & ~C_QU);
	else
		class = i;
	if (class != C_IN)
		printf(" %s", tok2str(ns_class2str, "(Class %d)", class));
	if (is_mdns) {
		if (i & C_QU)
			printf(" (QU)");
		else
			printf(" (QM)");
	}

	fputs("? ", stdout);
	cp = ns_nprint(np, bp);
	return(cp ? cp + 4 : NULL);
}

static const u_char *
ns_rprint(register const u_char *cp, register const u_char *bp, int is_mdns)
{
	register u_int i, class, opt_flags = 0;
	register u_short typ, len;
	register const u_char *rp;

	if (vflag) {
		putchar(' ');
		if ((cp = ns_nprint(cp, bp)) == NULL)
			return NULL;
	} else
		cp = ns_nskip(cp);

	if (cp == NULL || !TTEST2(*cp, 10))
		return (snapend);

	
	typ = EXTRACT_16BITS(cp);
	cp += 2;
	
	i = EXTRACT_16BITS(cp);
	cp += 2;
	if (is_mdns)
		class = (i & ~C_CACHE_FLUSH);
	else
		class = i;
	if (class != C_IN && typ != T_OPT)
		printf(" %s", tok2str(ns_class2str, "(Class %d)", class));
	if (is_mdns) {
		if (i & C_CACHE_FLUSH)
			printf(" (Cache flush)");
	}

	if (typ == T_OPT) {
		
		cp += 2;
		opt_flags = EXTRACT_16BITS(cp);
		
		cp += 2;
	} else if (vflag > 2) {
		
		printf(" [");
		relts_print(EXTRACT_32BITS(cp));
		printf("]");
		cp += 4;
	} else {
		
		cp += 4;
	}

	len = EXTRACT_16BITS(cp);
	cp += 2;

	rp = cp + len;

	printf(" %s", tok2str(ns_type2str, "Type%d", typ));
	if (rp > snapend)
		return(NULL);

	switch (typ) {
	case T_A:
		if (!TTEST2(*cp, sizeof(struct in_addr)))
			return(NULL);
		printf(" %s", intoa(htonl(EXTRACT_32BITS(cp))));
		break;

	case T_NS:
	case T_CNAME:
	case T_PTR:
#ifdef T_DNAME
	case T_DNAME:
#endif
		putchar(' ');
		if (ns_nprint(cp, bp) == NULL)
			return(NULL);
		break;

	case T_SOA:
		if (!vflag)
			break;
		putchar(' ');
		if ((cp = ns_nprint(cp, bp)) == NULL)
			return(NULL);
		putchar(' ');
		if ((cp = ns_nprint(cp, bp)) == NULL)
			return(NULL);
		if (!TTEST2(*cp, 5 * 4))
			return(NULL);
		printf(" %u", EXTRACT_32BITS(cp));
		cp += 4;
		printf(" %u", EXTRACT_32BITS(cp));
		cp += 4;
		printf(" %u", EXTRACT_32BITS(cp));
		cp += 4;
		printf(" %u", EXTRACT_32BITS(cp));
		cp += 4;
		printf(" %u", EXTRACT_32BITS(cp));
		cp += 4;
		break;
	case T_MX:
		putchar(' ');
		if (!TTEST2(*cp, 2))
			return(NULL);
		if (ns_nprint(cp + 2, bp) == NULL)
			return(NULL);
		printf(" %d", EXTRACT_16BITS(cp));
		break;

	case T_TXT:
		while (cp < rp) {
			printf(" \"");
			cp = ns_cprint(cp);
			if (cp == NULL)
				return(NULL);
			putchar('"');
		}
		break;

	case T_SRV:
		putchar(' ');
		if (!TTEST2(*cp, 6))
			return(NULL);
		if (ns_nprint(cp + 6, bp) == NULL)
			return(NULL);
		printf(":%d %d %d", EXTRACT_16BITS(cp + 4),
			EXTRACT_16BITS(cp), EXTRACT_16BITS(cp + 2));
		break;

#ifdef INET6
	case T_AAAA:
	    {
		struct in6_addr addr;
		char ntop_buf[INET6_ADDRSTRLEN];

		if (!TTEST2(*cp, sizeof(struct in6_addr)))
			return(NULL);
		memcpy(&addr, cp, sizeof(struct in6_addr));
		printf(" %s",
		    inet_ntop(AF_INET6, &addr, ntop_buf, sizeof(ntop_buf)));

		break;
	    }

	case T_A6:
	    {
		struct in6_addr a;
		int pbit, pbyte;
		char ntop_buf[INET6_ADDRSTRLEN];

		if (!TTEST2(*cp, 1))
			return(NULL);
		pbit = *cp;
		pbyte = (pbit & ~7) / 8;
		if (pbit > 128) {
			printf(" %u(bad plen)", pbit);
			break;
		} else if (pbit < 128) {
			if (!TTEST2(*(cp + 1), sizeof(a) - pbyte))
				return(NULL);
			memset(&a, 0, sizeof(a));
			memcpy(&a.s6_addr[pbyte], cp + 1, sizeof(a) - pbyte);
			printf(" %u %s", pbit,
			    inet_ntop(AF_INET6, &a, ntop_buf, sizeof(ntop_buf)));
		}
		if (pbit > 0) {
			putchar(' ');
			if (ns_nprint(cp + 1 + sizeof(a) - pbyte, bp) == NULL)
				return(NULL);
		}
		break;
	    }
#endif 

	case T_OPT:
		printf(" UDPsize=%u", class);
		if (opt_flags & 0x8000)
			printf(" OK");
		break;

	case T_UNSPECA:		
		if (!TTEST2(*cp, len))
			return(NULL);
		if (fn_printn(cp, len, snapend))
			return(NULL);
		break;

	case T_TSIG:
	    {
		if (cp + len > snapend)
			return(NULL);
		if (!vflag)
			break;
		putchar(' ');
		if ((cp = ns_nprint(cp, bp)) == NULL)
			return(NULL);
		cp += 6;
		if (!TTEST2(*cp, 2))
			return(NULL);
		printf(" fudge=%u", EXTRACT_16BITS(cp));
		cp += 2;
		if (!TTEST2(*cp, 2))
			return(NULL);
		printf(" maclen=%u", EXTRACT_16BITS(cp));
		cp += 2 + EXTRACT_16BITS(cp);
		if (!TTEST2(*cp, 2))
			return(NULL);
		printf(" origid=%u", EXTRACT_16BITS(cp));
		cp += 2;
		if (!TTEST2(*cp, 2))
			return(NULL);
		printf(" error=%u", EXTRACT_16BITS(cp));
		cp += 2;
		if (!TTEST2(*cp, 2))
			return(NULL);
		printf(" otherlen=%u", EXTRACT_16BITS(cp));
		cp += 2;
	    }
	}
	return (rp);		
}

void
ns_print(register const u_char *bp, u_int length, int is_mdns)
{
	register const HEADER *np;
	register int qdcount, ancount, nscount, arcount;
	register const u_char *cp;
	u_int16_t b2;

	np = (const HEADER *)bp;
	TCHECK(*np);
	
	qdcount = EXTRACT_16BITS(&np->qdcount);
	ancount = EXTRACT_16BITS(&np->ancount);
	nscount = EXTRACT_16BITS(&np->nscount);
	arcount = EXTRACT_16BITS(&np->arcount);

	if (DNS_QR(np)) {
		
		printf("%d%s%s%s%s%s%s",
			EXTRACT_16BITS(&np->id),
			ns_ops[DNS_OPCODE(np)],
			ns_resp[DNS_RCODE(np)],
			DNS_AA(np)? "*" : "",
			DNS_RA(np)? "" : "-",
			DNS_TC(np)? "|" : "",
			DNS_AD(np)? "$" : "");

		if (qdcount != 1)
			printf(" [%dq]", qdcount);
		
		cp = (const u_char *)(np + 1);
		while (qdcount--) {
			if (qdcount < EXTRACT_16BITS(&np->qdcount) - 1)
				putchar(',');
			if (vflag > 1) {
				fputs(" q:", stdout);
				if ((cp = ns_qprint(cp, bp, is_mdns)) == NULL)
					goto trunc;
			} else {
				if ((cp = ns_nskip(cp)) == NULL)
					goto trunc;
				cp += 4;	
			}
		}
		printf(" %d/%d/%d", ancount, nscount, arcount);
		if (ancount--) {
			if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
				goto trunc;
			while (cp < snapend && ancount--) {
				putchar(',');
				if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
					goto trunc;
			}
		}
		if (ancount > 0)
			goto trunc;
		
		if (vflag > 1) {
			if (cp < snapend && nscount--) {
				fputs(" ns:", stdout);
				if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
					goto trunc;
				while (cp < snapend && nscount--) {
					putchar(',');
					if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
						goto trunc;
				}
			}
			if (nscount > 0)
				goto trunc;
			if (cp < snapend && arcount--) {
				fputs(" ar:", stdout);
				if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
					goto trunc;
				while (cp < snapend && arcount--) {
					putchar(',');
					if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
						goto trunc;
				}
			}
			if (arcount > 0)
				goto trunc;
		}
	}
	else {
		
		printf("%d%s%s%s", EXTRACT_16BITS(&np->id), ns_ops[DNS_OPCODE(np)],
		    DNS_RD(np) ? "+" : "",
		    DNS_CD(np) ? "%" : "");

		
		b2 = EXTRACT_16BITS(((u_short *)np)+1);
		if (b2 & 0x6cf)
			printf(" [b2&3=0x%x]", b2);

		if (DNS_OPCODE(np) == IQUERY) {
			if (qdcount)
				printf(" [%dq]", qdcount);
			if (ancount != 1)
				printf(" [%da]", ancount);
		}
		else {
			if (ancount)
				printf(" [%da]", ancount);
			if (qdcount != 1)
				printf(" [%dq]", qdcount);
		}
		if (nscount)
			printf(" [%dn]", nscount);
		if (arcount)
			printf(" [%dau]", arcount);

		cp = (const u_char *)(np + 1);
		if (qdcount--) {
			cp = ns_qprint(cp, (const u_char *)np, is_mdns);
			if (!cp)
				goto trunc;
			while (cp < snapend && qdcount--) {
				cp = ns_qprint((const u_char *)cp,
					       (const u_char *)np,
					       is_mdns);
				if (!cp)
					goto trunc;
			}
		}
		if (qdcount > 0)
			goto trunc;

		
		if (vflag > 1) {
			if (ancount--) {
				if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
					goto trunc;
				while (cp < snapend && ancount--) {
					putchar(',');
					if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
						goto trunc;
				}
			}
			if (ancount > 0)
				goto trunc;
			if (cp < snapend && nscount--) {
				fputs(" ns:", stdout);
				if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
					goto trunc;
				while (nscount-- && cp < snapend) {
					putchar(',');
					if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
						goto trunc;
				}
			}
			if (nscount > 0)
				goto trunc;
			if (cp < snapend && arcount--) {
				fputs(" ar:", stdout);
				if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
					goto trunc;
				while (cp < snapend && arcount--) {
					putchar(',');
					if ((cp = ns_rprint(cp, bp, is_mdns)) == NULL)
						goto trunc;
				}
			}
			if (arcount > 0)
				goto trunc;
		}
	}
	printf(" (%d)", length);
	return;

  trunc:
	printf("[|domain]");
	return;
}