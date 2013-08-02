

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Fri Aug 02 19:59:24 2013
 */
/* Compiler settings for WallOfText.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, app_config, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#include <string.h>

#include "WallOfText_h.h"

#define TYPE_FORMAT_STRING_SIZE   75                                
#define PROC_FORMAT_STRING_SIZE   151                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _WallOfText_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } WallOfText_MIDL_TYPE_FORMAT_STRING;

typedef struct _WallOfText_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } WallOfText_MIDL_PROC_FORMAT_STRING;

typedef struct _WallOfText_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } WallOfText_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const WallOfText_MIDL_TYPE_FORMAT_STRING WallOfText__MIDL_TypeFormatString;
extern const WallOfText_MIDL_PROC_FORMAT_STRING WallOfText__MIDL_ProcFormatString;
extern const WallOfText_MIDL_EXPR_FORMAT_STRING WallOfText__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: WallOfText, ver. 1.0,
   GUID={0x3e64c978,0xf038,0x46f0,{0xbb,0x41,0x12,0x50,0xef,0xab,0x6b,0xc5}} */

handle_t wallOfText_IfHandle;


static const RPC_CLIENT_INTERFACE WallOfText___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x3e64c978,0xf038,0x46f0,{0xbb,0x41,0x12,0x50,0xef,0xab,0x6b,0xc5}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE WallOfText_v1_0_c_ifspec = (RPC_IF_HANDLE)& WallOfText___RpcClientInterface;

extern const MIDL_STUB_DESC WallOfText_StubDesc;

static RPC_BINDING_HANDLE WallOfText__MIDL_AutoBindHandle;


unsigned char ping( 
    /* [in] */ unsigned char testValue)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&WallOfText_StubDesc,
                  (PFORMAT_STRING) &WallOfText__MIDL_ProcFormatString.Format[0],
                  ( unsigned char * )&testValue);
    return ( unsigned char  )_RetVal.Simple;
    
}


void addMessage( 
    /* [in] */ pMessage newMessage)
{

    NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&WallOfText_StubDesc,
                  (PFORMAT_STRING) &WallOfText__MIDL_ProcFormatString.Format[36],
                  ( unsigned char * )&newMessage);
    
}


int getMessage( 
    /* [in] */ unsigned int messageId,
    /* [out] */ pMessage outMessage)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&WallOfText_StubDesc,
                  (PFORMAT_STRING) &WallOfText__MIDL_ProcFormatString.Format[66],
                  ( unsigned char * )&messageId);
    return ( int  )_RetVal.Simple;
    
}


unsigned int getLastMessages( 
    /* [in] */ unsigned int n,
    /* [size_is][out] */ pMessages messages)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&WallOfText_StubDesc,
                  (PFORMAT_STRING) &WallOfText__MIDL_ProcFormatString.Format[108],
                  ( unsigned char * )&n);
    return ( unsigned int  )_RetVal.Simple;
    
}


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const WallOfText_MIDL_PROC_FORMAT_STRING WallOfText__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure ping */

			0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 10 */	NdrFcShort( 0x5 ),	/* 5 */
/* 12 */	NdrFcShort( 0x5 ),	/* 5 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter testValue */

/* 24 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 26 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 28 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 34 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Procedure addMessage */

/* 36 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x1 ),	/* 1 */
/* 44 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x0 ),	/* 0 */
/* 50 */	0x42,		/* Oi2 Flags:  clt must size, has ext, */
			0x1,		/* 1 */
/* 52 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newMessage */

/* 60 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 62 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 64 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Procedure getMessage */

/* 66 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 68 */	NdrFcLong( 0x0 ),	/* 0 */
/* 72 */	NdrFcShort( 0x2 ),	/* 2 */
/* 74 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 76 */	NdrFcShort( 0x8 ),	/* 8 */
/* 78 */	NdrFcShort( 0x8 ),	/* 8 */
/* 80 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 82 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 84 */	NdrFcShort( 0x0 ),	/* 0 */
/* 86 */	NdrFcShort( 0x0 ),	/* 0 */
/* 88 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter messageId */

/* 90 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 92 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 94 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter outMessage */

/* 96 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure getLastMessages */

/* 108 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0x3 ),	/* 3 */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	NdrFcShort( 0x8 ),	/* 8 */
/* 120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 122 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 124 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 126 */	NdrFcShort( 0x1 ),	/* 1 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter n */

/* 132 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 134 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 136 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter messages */

/* 138 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 140 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 142 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Return value */

/* 144 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 146 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 148 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const WallOfText_MIDL_TYPE_FORMAT_STRING WallOfText__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x2,	/* FC_RP [dont_free] */
/*  4 */	NdrFcShort( 0x2 ),	/* Offset= 2 (6) */
/*  6 */	
			0x16,		/* FC_PSTRUCT */
			0x7,		/* 7 */
/*  8 */	NdrFcShort( 0x10 ),	/* 16 */
/* 10 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 12 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 14 */	NdrFcShort( 0x8 ),	/* 8 */
/* 16 */	NdrFcShort( 0x8 ),	/* 8 */
/* 18 */	0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 20 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */
/* 22 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 24 */	NdrFcShort( 0xc ),	/* 12 */
/* 26 */	NdrFcShort( 0xc ),	/* 12 */
/* 28 */	0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 30 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */
/* 32 */	
			0x5b,		/* FC_END */

			0xb,		/* FC_HYPER */
/* 34 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 36 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 38 */	
			0x11, 0x2,	/* FC_RP [dont_free] */
/* 40 */	NdrFcShort( 0x2 ),	/* Offset= 2 (42) */
/* 42 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 44 */	NdrFcShort( 0x4 ),	/* 4 */
/* 46 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 48 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 50 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 52 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 54 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 56 */	NdrFcShort( 0x4 ),	/* 4 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0x1 ),	/* 1 */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */
/* 66 */	0x12, 0x0,	/* FC_UP */
/* 68 */	NdrFcShort( 0xffc2 ),	/* Offset= -62 (6) */
/* 70 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 72 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const unsigned short WallOfText_FormatStringOffsetTable[] =
    {
    0,
    36,
    66,
    108
    };


static const MIDL_STUB_DESC WallOfText_StubDesc = 
    {
    (void *)& WallOfText___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &wallOfText_IfHandle,
    0,
    0,
    0,
    0,
    WallOfText__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x8000253, /* MIDL Version 8.0.595 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

