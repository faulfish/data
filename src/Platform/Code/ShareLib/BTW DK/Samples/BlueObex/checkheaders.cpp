/*****************************************************************************
**
**  Name:       checkheaders.cpp
**
**  File:       Verifies the methods in  CObexHeaders
**
**  Date        Modification
**  ------------------------ 
**  5Jun2001    Jim French  Create
**
**  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.
**
*****************************************************************************/
#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "checkheaders.h"

ObexCheckHeaders::~ObexCheckHeaders()
{
}

ObexCheckHeaders::ObexCheckHeaders()
{
}

// test values to set in headers with Set* and Add* functions
// and to verify using Get* and Delete* functions in CObexHeaders
//
static UINT32 val_count = 5;
static WCHAR val_name[] = L"name header";
static unsigned char val_type[] = "type header";
static UINT32 val_length = 1000;
static char val_time[] = "20011225T235959";
static WCHAR val_description[] = L"description header";
static char val_target_base[] = "target ";
static int val_target_base_len = (int) strlen (val_target_base);
static char val_http_base[] = "HTTP ";
static int val_http_base_len = (int) strlen (val_http_base);
static unsigned char val_body[] = "body header";
static BOOL val_body_end = TRUE;
static unsigned char val_who[] = "who header";
static char val_app_param_base[] = "APP PARAM ";
static int val_app_param_base_len = (int) strlen (val_app_param_base);
static UINT8 val_app_param_tag_base = 200;
static char val_auth_challenge_base[] = "AUTH CHALLENGE ";
static int val_auth_challenge_base_len = (int) strlen (val_auth_challenge_base);
static UINT8 val_auth_challenge_tag_base = 210;
static char val_auth_response_base[] = "AUTH RESPONSE ";
static int val_auth_response_base_len = (int) strlen (val_auth_response_base);
static UINT8 val_auth_response_tag_base = 230;
static unsigned char val_object_class[] = "object class header";

// for user defined header test pick one of the valid types for variable select_user_defined_type
// OBEX_USER_TYPE_UNI  - Null terminated UNICODE text
// OBEX_USER_TYPE_ARRAY  - Octet array
// OBEX_USER_TYPE_BYTE  - Single byte
// OBEX_USER_TYPE_INT   - 32 bit integer

static UINT8 select_user_defined_type = OBEX_USER_TYPE_BYTE;
static WCHAR user_defined_string[] = L"user defined string";
static UINT8 user_defined_array[] = "user defined array";
static UINT8 user_defined_byte = 150;
static UINT32 user_defined_int = 1234567;
static UINT8 user_defined_id = 50;
static CObexUserDefined val_user_defined;

	
CString ObexCheckHeaders::Fill()
{
	// put a value in every headers field
	// return with an error message on the first failure 

	// count header
	hdrs.SetCount(val_count);

	// name header
	if (!hdrs.SetName(val_name))
		return ("FAIL SetName");

	// type header
	if (!hdrs.SetType(val_type, sizeof(val_type)))
		return ("FAIL SetType");

	// length header
	hdrs.SetLength(val_length);

	// time header
	if (!hdrs.SetTime(val_time))
		return ("FAIL SetTime");
	
	// description header
	if (!hdrs.SetDescription(val_description))
		return ("FAIL SetDescription");

	// target header
	{
		int ii;
		for (ii = 0; ii < OBEX_MAX_TARGET; ii++)
		{
			UINT32 length = 0;
			if (hdrs.GetTargetLength(&length, ii))
				return ("FAIL target should not exist");

			UINT8 *p_val_in = (UINT8 *)malloc (val_target_base_len + OBEX_MAX_TARGET + 1);
			if (p_val_in == NULL)
				return ("FAIL target setup in add");
			strcpy((char *)p_val_in, val_target_base);
			UINT8 suffix[2];
			sprintf((char *)suffix,"%d", ii + 1);	// suffix shows which element 

			// make each target different value and length
			for (int jj = 0; jj < ii + 1; jj++)
				strcat((char *)p_val_in, (char *)suffix);
			if (!hdrs.AddTarget (p_val_in, (UINT32) strlen((char *)p_val_in)))
			{
				free (p_val_in);
				return ("FAIL AddTarget ");
			}
			free(p_val_in);

			// check count 
			if (hdrs.GetTargetCnt() != (UINT32)(ii+1))
				return ("FAIL GetTargetCnt");
		}

		// see if the method rejects too many entries
		if (hdrs.AddTarget((UINT8 *)val_target_base, val_target_base_len))
			return ("FAIL AddTarget allowed too many entries");

		// verify the final count of entries
		UINT32 cnt = hdrs.GetTargetCnt();
		if (cnt != OBEX_MAX_TARGET)
			return ("FAIL target final count");

	}

	// http header
	{
		int ii;
		for (ii = 0; ii < OBEX_MAX_HTTP; ii++)
		{
			UINT32 length = 0;
			if (hdrs.GetHttpLength(&length, ii))
				return ("FAIL http should not exist");

			UINT8 *p_val_in = (UINT8 *)malloc (val_http_base_len + OBEX_MAX_HTTP + 1);
			if (p_val_in == NULL)
				return ("FAIL http setup in add");
			strcpy((char *)p_val_in, val_http_base);
			UINT8 suffix[2];
			sprintf((char *)suffix,"%d", ii + 1);	// suffix shows which element 

			// make each http different value and length
			for (int jj = 0; jj < ii + 1; jj++)
				strcat((char *)p_val_in, (char *)suffix);
			if (!hdrs.AddHttp (p_val_in, (UINT32) strlen((char *)p_val_in)))
			{
				free (p_val_in);
				return ("FAIL AddHttp ");
			}
			free(p_val_in);

			// check count 
			if (hdrs.GetHttpCnt() != (UINT32)(ii+1))
				return ("FAIL GetHttpCnt");
		}

		// see if the method rejects too many entries
		if (hdrs.AddHttp((UINT8 *)val_http_base, val_http_base_len))
			return ("FAIL AddHttp allowed too many entries");

		// verify the final count of entries
		UINT32 cnt = hdrs.GetHttpCnt();
		if (cnt != OBEX_MAX_HTTP)
			return ("FAIL http final count");

	}
	
	// body header
	if (!hdrs.SetBody(val_body, sizeof(val_body), val_body_end))
		return ("FAIL SetBody");

	// who header
	if (!hdrs.SetWho(val_who, sizeof(val_who)))
		return ("FAIL SetWho");

	// app param header
			UINT8 suffix[2];
	{
		int ii;
		for (ii = 0; ii < OBEX_MAX_APP_PARAM; ii++)
		{
			UINT8 length = 0;
			if (hdrs.GetAppParamLength(&length, ii))
				return ("FAIL app param should not exist");

			UINT8 *p_val_in = (UINT8 *)malloc (val_app_param_base_len + OBEX_MAX_APP_PARAM + 1);
			if (p_val_in == NULL)
				return ("FAIL app param setup in add");
			strcpy((char *)p_val_in, val_app_param_base);
			sprintf((char *)suffix,"%d", ii + 1);	// suffix shows which element 

			// make each http different value and length
			for (UINT8 jj = 0; jj < ii + 1; jj++)
				strcat((char *)p_val_in, (char *)suffix);
			if (!hdrs.AddAppParam (val_app_param_tag_base + ii, (UINT8) strlen((char *)p_val_in), p_val_in))
			{
				free (p_val_in);
				return ("FAIL AddAppParam ");
			}
			free(p_val_in);

			// check count 
			if (hdrs.GetAppParamCnt() != (UINT32)(ii+1))
				return ("FAIL GetAppParamCnt");
		}

		// see if the method rejects too many entries
		if (hdrs.AddAppParam(val_app_param_tag_base, val_app_param_base_len, (UINT8 *)val_app_param_base))
			return ("FAIL AddAppParam allowed too many entries");

		// verify the final count of entries
		UINT32 cnt = hdrs.GetAppParamCnt();
		if (cnt != OBEX_MAX_APP_PARAM)
			return ("FAIL app param final count");

	}

	// auth challenge header
	{
		int ii;
		for (ii = 0; ii < OBEX_MAX_AUTH_CHALLENGE; ii++)
		{
			UINT8 length = 0;
			if (hdrs.GetAuthChallengeLength(&length, ii))
				return ("FAIL auth challenge should not exist");

			UINT8 *p_val_in = (UINT8 *)malloc (val_auth_challenge_base_len + OBEX_MAX_AUTH_CHALLENGE + 1);
			if (p_val_in == NULL)
				return ("FAIL auth challenge setup in add");
			strcpy((char *)p_val_in, val_auth_challenge_base);
			sprintf((char *)suffix,"%d", ii + 1);	// suffix shows which element 

			// make each http different value and length
			for (UINT8 jj = 0; jj < ii + 1; jj++)
				strcat((char *)p_val_in, (char *)suffix);
			if (!hdrs.AddAuthChallenge (val_auth_challenge_tag_base + ii, (UINT8) strlen((char *)p_val_in), p_val_in))
			{
				free (p_val_in);
				return ("FAIL AddAuthChallenge ");
			}
			free(p_val_in);

			// check count 
			if (hdrs.GetAuthChallengeCnt() != (UINT32)(ii+1))
				return ("FAIL GetAuthChallengeCnt");
		}

		// see if the method rejects too many entries
		if (hdrs.AddAuthChallenge(val_auth_challenge_tag_base, val_auth_challenge_base_len, (UINT8 *)val_auth_challenge_base))
			return ("FAIL AddAuthChallenge allowed too many entries");

		// verify the final count of entries
		UINT32 cnt = hdrs.GetAuthChallengeCnt();
		if (cnt != OBEX_MAX_AUTH_CHALLENGE)
			return ("FAIL auth challenge final count");

	}

	// auth response header
	{
		int ii;
		for (ii = 0; ii < OBEX_MAX_AUTH_RESPONSE; ii++)
		{
			UINT8 length = 0;
			if (hdrs.GetAuthResponseLength(&length, ii))
				return ("FAIL auth response should not exist");

			UINT8 *p_val_in = (UINT8 *)malloc (val_auth_response_base_len + OBEX_MAX_AUTH_RESPONSE + 1);
			if (p_val_in == NULL)
				return ("FAIL auth response setup in add");
			strcpy((char *)p_val_in, val_auth_response_base);
			sprintf((char *)suffix,"%d", ii + 1);	// suffix shows which element 

			// make each http different value and length
			for (UINT8 jj = 0; jj < ii + 1; jj++)
				strcat((char *)p_val_in, (char *)suffix);
			if (!hdrs.AddAuthResponse (val_auth_response_tag_base + ii, (UINT8) strlen((char *)p_val_in), p_val_in))
			{
				free (p_val_in);
				return ("FAIL AddAuthResponse ");
			}
			free(p_val_in);

			// check count 
			if (hdrs.GetAuthResponseCnt() != (UINT32)(ii+1))
				return ("FAIL GetAuthResponseCnt");
		}

		// see if the method rejects too many entries
		if (hdrs.AddAuthResponse(val_auth_response_tag_base, val_auth_response_base_len, (UINT8 *)val_auth_response_base))
			return ("FAIL AddAuthResponse allowed too many entries");

		// verify the final count of entries
		UINT32 cnt = hdrs.GetAuthResponseCnt();
		if (cnt != OBEX_MAX_AUTH_RESPONSE)
			return ("FAIL auth response final count");

	}


	// object class
	if (!hdrs.SetObjectClass(val_object_class, sizeof(val_object_class)))
		return ("FAIL SetObjectClass");


	// user defined header
	{
		// set up a CObexUserDefined object based on the type of user defined object
		switch (select_user_defined_type)
		{
		case OBEX_USER_TYPE_UNI:
			val_user_defined.SetHeader(user_defined_id, user_defined_string);
			break;
		case OBEX_USER_TYPE_ARRAY:
			val_user_defined.SetHeader(user_defined_id, user_defined_array, (UINT16) strlen ((char *)user_defined_array));
			break;
		case OBEX_USER_TYPE_BYTE:
			val_user_defined.SetHeader(user_defined_id, user_defined_byte);
			break;
		case OBEX_USER_TYPE_INT:
			val_user_defined.SetHeader(user_defined_id, user_defined_int);
			break;
		default:
			return ("FAIL user define header, invalid object type selected for test");
		}

		// add the user defined object 
		for (int ii = 0; ii < OBEX_MAX_USER_HDR; ii++)
		{
			if (!hdrs.AddUserDefined(&val_user_defined))
				return ("FAIL AddUserDefined");
		}
		// attempt to add another should ne rejected
		if (hdrs.AddUserDefined(&val_user_defined))
			return ("FAIL AddUserDefined allowed too many entries");
	}

	// all headers filled with success
	return ("Set values OK for all headers");
}


CString ObexCheckHeaders::Verify()
{
	//////////////////////////////////////////////////////////////////////////////
	// Get every header field and compare with the value set by Fill().
	// Return with an error message on the frst failure

	// count header
	UINT32 val_count_out = 0;
	if (hdrs.GetCount(&val_count_out))
	{
		if (val_count != val_count_out)
			return ("FAIL count verify");
	}
	else
		return ("FAIL count present");

	// name header
	{
		UINT32 length = 0;
		WCHAR *p_array = NULL;

		if (hdrs.GetNameLength (&length))
		{
			if (wcslen(val_name) + 1 != length)
				return ("FAIL name length verify");
			p_array = (WCHAR *)malloc((length+1)*sizeof (WCHAR));
			if (!hdrs.GetName(p_array))
				return ("FAIL name present verify");
			if (wcsncmp(val_name, p_array, wcslen(val_name)))
			{
				free (p_array);
				return ("FAIL name verify");
			}
			free (p_array);
		}
		else
			return ("FAIL name present");
	}

	// type header
	{	unsigned char *p_octets = NULL;
		UINT32 length = 0;
		if (hdrs.GetTypeLength(&length))
		{
			if (sizeof(val_type) != length)
				return ("FAIL type length verify");
			p_octets = (unsigned char *)malloc(length);
			if (!hdrs.GetType(p_octets))
				return ("FAIL type present verify");
			if (memcmp(val_type, p_octets, sizeof(val_type)))
			{
				free (p_octets);
				return ("FAIL type verify");
			}
			free (p_octets);
		}
		else
			return ("FAIL type present");
	}

	// length header
	UINT32 val_length_out = 0;
	if (hdrs.GetLength(&val_length_out))
	{
		if (val_length != val_length_out)
			return ("FAIL length verify");
	}
	else
		return ("FAIL length present");

	// time header
	{
		char time_str[17] = "";
		if (!hdrs.GetTime(time_str))
			return ("FAIL time present");

		if (strcmp(time_str, val_time) != 0)
			return ("FAIL time verify");
	}

	// description header
	{
		UINT32 length = 0;
		WCHAR *p_array = NULL;

		if (hdrs.GetDescriptionLength (&length))
		{
			if (wcslen(val_description) + 1 != length)
				return ("FAIL description length verify");
			p_array = (WCHAR *)malloc((length+1)*sizeof(WCHAR));
			if (!hdrs.GetDescription(p_array))
				return ("FAIL description present verify");
			if (wcsncmp(val_description, p_array, wcslen(val_description)))
			{
				free (p_array);
				return ("FAIL description verify");
			}
			free (p_array);
		}
		else
			return ("FAIL description present");
	}

	// target header
	{
		int ii;
		UINT32 length_out = 0;
		BOOL failed = FALSE;
		UINT8 *p_val_out = NULL;
		UINT8 *p_val_in = NULL;
		char err_msg[256];

		for (ii = 0; ii < OBEX_MAX_TARGET; ii++)
		{
			if (!hdrs.GetTargetLength(&length_out, ii))
			{
				sprintf(err_msg, "FAIL target %d should exist", ii + 1);
				failed = TRUE;
				break;
			}

			p_val_out = (UINT8 *)malloc (length_out);
			if (p_val_out == NULL)
			{
				sprintf(err_msg, "FAIL GetTarget %d malloc fail in verify ", ii + 1);
				failed = TRUE;
				break;
			}

			// fetch the value for the current index
			if ( !hdrs.GetTarget(p_val_out, ii))
			{
				sprintf(err_msg, "FAIL GetTarget %d", ii + 1);
				failed = TRUE;
				break;
			}
			
			// build expected value the same as when added
			p_val_in = (UINT8 *)malloc (val_target_base_len + OBEX_MAX_TARGET + 1);
			if (p_val_in == NULL)
			{
				sprintf(err_msg, "FAIL GetTarget %d malloc in compare", ii + 1);
				failed = TRUE;
				break;
			}

			strcpy((char *)p_val_in, val_target_base);
			UINT8 suffix[2];
			sprintf((char *)suffix,"%d", ii + 1);	// suffix shows which element 

			// make each target different value and length
			for (int jj = 0; jj < ii + 1; jj++)
				strcat((char *)p_val_in, (char *)suffix);

			// they should match in length and content, fail if not
			if (length_out != strlen((char *)p_val_in))
			{
				sprintf(err_msg, "FAIL target %d length in compare", ii + 1);
				failed = TRUE;
				break;
			}

			if (strncmp((char *)p_val_in, (char *)p_val_out, length_out) != 0)
			{
				sprintf(err_msg, "FAIL target %d match in compare", ii + 1);
				failed = TRUE;
				break;
			}

			// free malloced arrays
			if (p_val_in != NULL)
			{
				free (p_val_in);
				p_val_in = NULL;
			}
			if (p_val_out != NULL)
			{
				free (p_val_out);
				p_val_out = NULL;
			}
		}

		// free malloced arrays
		if (p_val_in != NULL)
			free (p_val_in);
		if (p_val_out != NULL)
			free (p_val_out);

		if (failed)
			return (err_msg);
	}

	// http header
	{
		int ii;
		UINT32 length_out = 0;
		BOOL failed = FALSE;
		UINT8 *p_val_out = NULL;
		UINT8 *p_val_in = NULL;
		char err_msg[256];

		for (ii = 0; ii < OBEX_MAX_HTTP; ii++)
		{
			if (!hdrs.GetHttpLength(&length_out, ii))
			{
				sprintf(err_msg, "FAIL http %d should exist", ii + 1);
				failed = TRUE;
				break;
			}

			p_val_out = (UINT8 *)malloc (length_out);
			if (p_val_out == NULL)
			{
				sprintf(err_msg, "FAIL GetHttp %d malloc fail in verify ", ii + 1);
				failed = TRUE;
				break;
			}

			// fetch the value for the current index
			if ( !hdrs.GetHttp(p_val_out, ii))
			{
				sprintf(err_msg, "FAIL GetHttp %d", ii + 1);
				failed = TRUE;
				break;
			}
			
			// build expected value the same as when added
			p_val_in = (UINT8 *)malloc (val_http_base_len + OBEX_MAX_HTTP + 1);
			if (p_val_in == NULL)
			{
				sprintf(err_msg, "FAIL GetHttp %d malloc in compare", ii + 1);
				failed = TRUE;
				break;
			}

			strcpy((char *)p_val_in, val_http_base);
			UINT8 suffix[2];
			sprintf((char *)suffix,"%d", ii + 1);	// suffix shows which element 

			// make each http different value and length
			for (int jj = 0; jj < ii + 1; jj++)
				strcat((char *)p_val_in, (char *)suffix);

			// they should match in length and content, fail if not
			if (length_out != strlen((char *)p_val_in))
			{
				sprintf(err_msg, "FAIL http %d length in compare", ii + 1);
				failed = TRUE;
				break;
			}

			if (strncmp((char *)p_val_in, (char *)p_val_out, length_out) != 0)
			{
				sprintf(err_msg, "FAIL http %d match in compare", ii + 1);
				failed = TRUE;
				break;
			}

			// free malloced arrays
			if (p_val_in != NULL)
			{
				free (p_val_in);
				p_val_in = NULL;
			}
			if (p_val_out != NULL)
			{
				free (p_val_out);
				p_val_out = NULL;
			}
		}

		// free malloced arrays
		if (p_val_in != NULL)
			free (p_val_in);
		if (p_val_out != NULL)
			free (p_val_out);

		if (failed)
			return (err_msg);
	}

	// body header
	{	unsigned char *p_octets = NULL;
		UINT32 length = 0;

		if (hdrs.GetBodyLength(&length))
		{
			BOOL body_end_out;
			if (sizeof(val_body) != length)
				return ("FAIL body length verify");
			p_octets = (unsigned char *)malloc(length);
			if (!hdrs.GetBody(p_octets, &body_end_out))
			{
				free (p_octets);
				return ("FAIL body present verify");
			}
			if (val_body_end != body_end_out)
			{
				free (p_octets);
				return ("FAIL body end flag verify");
			}
			if (memcmp(val_body, p_octets, sizeof(val_body)))
			{
				free (p_octets);
				return ("FAIL body verify");
			}
			free (p_octets);
		}
		else
			return ("FAIL body present");
	}
	
	// who header
	{	unsigned char *p_octets = NULL;
		UINT32 length = 0;
		if (hdrs.GetWhoLength(&length))
		{
			if (sizeof(val_who) != length)
				return ("FAIL who length verify");
			p_octets = (unsigned char *)malloc(length);
			if (!hdrs.GetWho(p_octets))
				return ("FAIL who present verify");
			if (memcmp(val_who, p_octets, sizeof(val_who)))
			{
				free (p_octets);
				return ("FAIL who verify");
			}
			free (p_octets);
		}
		else
			return ("FAIL who present");
	}

	// app param header
	{
		int ii;
		UINT8 length_out = 0;
		BOOL failed = FALSE;
		UINT8 *p_val_out = NULL;
		UINT8 *p_val_in = NULL;
		UINT8 tag;
		char err_msg[256];

		for (ii = 0; ii < OBEX_MAX_APP_PARAM; ii++)
		{
			if (!hdrs.GetAppParamLength(&length_out, ii))
			{
				sprintf(err_msg, "FAIL app param %d should exist", ii + 1);
				failed = TRUE;
				break;
			}

			p_val_out = (UINT8 *)malloc (length_out);
			if (p_val_out == NULL)
			{
				sprintf(err_msg, "FAIL GetAppParam %d malloc fail in verify ", ii + 1);
				failed = TRUE;
				break;
			}

			// fetch the value for the current index
			if ( !hdrs.GetAppParam(&tag, p_val_out, ii))
			{
				sprintf(err_msg, "FAIL GetAppParam %d", ii + 1);
				failed = TRUE;
				break;
			}

			// build expected value the same as when added
			p_val_in = (UINT8 *)malloc (val_app_param_base_len + OBEX_MAX_APP_PARAM + 1);
			if (p_val_in == NULL)
			{
				sprintf(err_msg, "FAIL GetAppParam %d malloc in compare", ii + 1);
				failed = TRUE;
				break;
			}

			strcpy((char *)p_val_in, val_app_param_base);
			UINT8 suffix[2];
			sprintf((char *)suffix,"%d", ii + 1);	// suffix shows which element 

			// make each http different value and length and each tag value different
			for (int jj = 0; jj < ii + 1; jj++)
				strcat((char *)p_val_in, (char *)suffix);

			// verify tag value; each is different
			if (tag != val_app_param_tag_base + ii)
			{
				sprintf(err_msg, "FAIL GetAppParam %d tag in compare", ii + 1);
				failed = TRUE;
				break;
			}

			// they should match in length and content, fail if not
			if (length_out != strlen((char *)p_val_in))
			{
				sprintf(err_msg, "FAIL app param %d length in compare", ii + 1);
				failed = TRUE;
				break;
			}

			if (strncmp((char *)p_val_in, (char *)p_val_out, length_out) != 0)
			{
				sprintf(err_msg, "FAIL app param %d match in compare", ii + 1);
				failed = TRUE;
				break;
			}

			// free malloced arrays
			if (p_val_in != NULL)
			{
				free (p_val_in);
				p_val_in = NULL;
			}
			if (p_val_out != NULL)
			{
				free (p_val_out);
				p_val_out = NULL;
			}
		}

		// free malloced arrays
		if (p_val_in != NULL)
			free (p_val_in);
		if (p_val_out != NULL)
			free (p_val_out);

		if (failed)
			return (err_msg);
	}

	// auth challenge header
	{
		int ii;
		UINT8 length_out = 0;
		BOOL failed = FALSE;
		UINT8 *p_val_out = NULL;
		UINT8 *p_val_in = NULL;
		UINT8 tag;
		char err_msg[256];

		for (ii = 0; ii < OBEX_MAX_AUTH_CHALLENGE; ii++)
		{
			if (!hdrs.GetAuthChallengeLength(&length_out, ii))
			{
				sprintf(err_msg, "FAIL auth challenge %d should exist", ii + 1);
				failed = TRUE;
				break;
			}

			p_val_out = (UINT8 *)malloc (length_out);
			if (p_val_out == NULL)
			{
				sprintf(err_msg, "FAIL GetAuthChallenge %d malloc fail in verify ", ii + 1);
				failed = TRUE;
				break;
			}

			// fetch the value for the current index
			if ( !hdrs.GetAuthChallenge(&tag, p_val_out, ii))
			{
				sprintf(err_msg, "FAIL GetAuthChallenge %d", ii + 1);
				failed = TRUE;
				break;
			}

			// build expected value the same as when added
			p_val_in = (UINT8 *)malloc (val_auth_challenge_base_len + OBEX_MAX_AUTH_CHALLENGE + 1);
			if (p_val_in == NULL)
			{
				sprintf(err_msg, "FAIL GetAuthChallenge %d malloc in compare", ii + 1);
				failed = TRUE;
				break;
			}

			strcpy((char *)p_val_in, val_auth_challenge_base);
			UINT8 suffix[2];
			sprintf((char *)suffix,"%d", ii + 1);	// suffix shows which element 

			// make each http different value and length and each tag value different
			for (int jj = 0; jj < ii + 1; jj++)
				strcat((char *)p_val_in, (char *)suffix);

			// verify tag value; each is different
			if (tag != val_auth_challenge_tag_base + ii)
			{
				sprintf(err_msg, "FAIL GetAuthChallenge %d tag in compare", ii + 1);
				failed = TRUE;
				break;
			}

			// they should match in length and content, fail if not
			if (length_out != strlen((char *)p_val_in))
			{
				sprintf(err_msg, "FAIL auth challenge %d length in compare", ii + 1);
				failed = TRUE;
				break;
			}

			if (strncmp((char *)p_val_in, (char *)p_val_out, length_out) != 0)
			{
				sprintf(err_msg, "FAIL auth challenge %d match in compare", ii + 1);
				failed = TRUE;
				break;
			}

			// free malloced arrays
			if (p_val_in != NULL)
			{
				free (p_val_in);
				p_val_in = NULL;
			}
			if (p_val_out != NULL)
			{
				free (p_val_out);
				p_val_out = NULL;
			}
		}

		// free malloced arrays
		if (p_val_in != NULL)
			free (p_val_in);
		if (p_val_out != NULL)
			free (p_val_out);

		if (failed)
			return (err_msg);
	}

	// auth response header
	{
		int ii;
		UINT8 length_out = 0;
		BOOL failed = FALSE;
		UINT8 *p_val_out = NULL;
		UINT8 *p_val_in = NULL;
		UINT8 tag;
		char err_msg[256];

		for (ii = 0; ii < OBEX_MAX_AUTH_RESPONSE; ii++)
		{
			if (!hdrs.GetAuthResponseLength(&length_out, ii))
			{
				sprintf(err_msg, "FAIL auth response %d should exist", ii + 1);
				failed = TRUE;
				break;
			}

			p_val_out = (UINT8 *)malloc (length_out);
			if (p_val_out == NULL)
			{
				sprintf(err_msg, "FAIL GetAuthResponse %d malloc fail in verify ", ii + 1);
				failed = TRUE;
				break;
			}

			// fetch the value for the current index
			if ( !hdrs.GetAuthResponse(&tag, p_val_out, ii))
			{
				sprintf(err_msg, "FAIL GetAuthResponse %d", ii + 1);
				failed = TRUE;
				break;
			}

			// build expected value the same as when added
			p_val_in = (UINT8 *)malloc (val_auth_response_base_len + OBEX_MAX_AUTH_RESPONSE + 1);
			if (p_val_in == NULL)
			{
				sprintf(err_msg, "FAIL GetAuthResponse %d malloc in compare", ii + 1);
				failed = TRUE;
				break;
			}

			strcpy((char *)p_val_in, val_auth_response_base);
			UINT8 suffix[2];
			sprintf((char *)suffix,"%d", ii + 1);	// suffix shows which element 

			// make each http different value and length and each tag value different
			for (int jj = 0; jj < ii + 1; jj++)
				strcat((char *)p_val_in, (char *)suffix);

			// verify tag value; each is different
			if (tag != val_auth_response_tag_base + ii)
			{
				sprintf(err_msg, "FAIL GetAuthResponse %d tag in compare", ii + 1);
				failed = TRUE;
				break;
			}

			// they should match in length and content, fail if not
			if (length_out != strlen((char *)p_val_in))
			{
				sprintf(err_msg, "FAIL auth response %d length in compare", ii + 1);
				failed = TRUE;
				break;
			}

			if (strncmp((char *)p_val_in, (char *)p_val_out, length_out) != 0)
			{
				sprintf(err_msg, "FAIL auth response %d match in compare", ii + 1);
				failed = TRUE;
				break;
			}

			// free malloced arrays
			if (p_val_in != NULL)
			{
				free (p_val_in);
				p_val_in = NULL;
			}
			if (p_val_out != NULL)
			{
				free (p_val_out);
				p_val_out = NULL;
			}
		}

		// free malloced arrays
		if (p_val_in != NULL)
			free (p_val_in);
		if (p_val_out != NULL)
			free (p_val_out);

		if (failed)
			return (err_msg);
	}

	// object class header
	{	unsigned char *p_octets = NULL;
		UINT32 length = 0;
		if (hdrs.GetObjectClassLength(&length))
		{
			if (sizeof(val_object_class) != length)
				return ("FAIL object class length verify");
			p_octets = (unsigned char *)malloc(length);
			if (!hdrs.GetObjectClass(p_octets))
				return ("FAIL object class present verify");
			if (memcmp(val_object_class, p_octets, sizeof(val_object_class)))
			{
				free (p_octets);
				return ("FAIL object class verify");
			}
			free (p_octets);
		}
		else
			return ("FAIL object class present");
	}

	// user defined header
	{
		BOOL failed = FALSE;
		UINT8 *p_val_array_out = NULL;
		WCHAR *p_val_string_out = NULL;
		char err_msg[256] = "";
		int ii;

		for (ii = 0; ii < OBEX_MAX_USER_HDR; ii++)
		{
			UINT16 length_out;

			CObexUserDefined user_defined_out;
			if (!hdrs.GetUserDefinedLength(&length_out, ii))
			{
				sprintf(err_msg, "FAIL user defined %d should exist to get length", ii + 1);
				failed = TRUE;
				break;
			}
			if (!hdrs.GetUserDefined(&user_defined_out, ii))
			{
				sprintf(err_msg, "FAIL user defined %d should exist to get CObexUserDefined object", ii + 1);
				failed = TRUE;
				break;
			}

			// verify content
			UINT8 user_type_out;
			UINT8 id_out;

			user_type_out = user_defined_out.GetUserType(&id_out);
			if (id_out != user_defined_id)
			{
				sprintf(err_msg, "FAIL user_defined %d type verify", ii + 1);
				break;
			}

			if (user_type_out != select_user_defined_type)
			{
				sprintf(err_msg, "FAIL user_defined %d id value verify", ii + 1);
				break;
			}


			switch(user_type_out)
			{
			case OBEX_USER_TYPE_UNI:
				length_out = user_defined_out.GetLength();
				p_val_string_out = (WCHAR *)malloc ((length_out+1)*sizeof(WCHAR));
				if (p_val_string_out == NULL)
				{
					sprintf(err_msg, "FAIL GetUserDefined %d malloc fail in verify text", ii + 1);
					failed = TRUE;
					break;
				}

				user_defined_out.GetText(p_val_string_out);

				// input aand output should match in length and content, fail if not
				if (length_out != wcslen(user_defined_string) + 1)	// include null terminator
				{
					sprintf(err_msg, "FAIL GetUserDefined %d text length compare", ii + 1);
					failed = TRUE;
					break;
				}

				if (wcsncmp(user_defined_string, p_val_string_out, length_out) != 0)
				{
					sprintf(err_msg, "FAIL GetUserDefined %d text compare", ii + 1);
					failed = TRUE;
					break;
				}
				break;

			case OBEX_USER_TYPE_ARRAY:
				length_out = user_defined_out.GetLength();
				p_val_array_out = (UINT8 *)malloc (length_out);
				if (p_val_array_out == NULL)
				{
					sprintf(err_msg, "FAIL GetUserDefined %d malloc fail in verify data", ii + 1);
					failed = TRUE;
					break;
				}

				user_defined_out.GetOctets(p_val_array_out);

				// input and output should match in length and content, fail if not
				if (length_out != strlen((char *)user_defined_array))
				{
					sprintf(err_msg, "FAIL GetUserDefined %d data length compare", ii + 1);
					failed = TRUE;
					break;
				}

				if (strncmp((char *)user_defined_array, (char *)p_val_array_out, length_out) != 0)
				{
					sprintf(err_msg, "FAIL GetUserDefined %d data compare", ii + 1);
					failed = TRUE;
					break;
				}
				break;

			case OBEX_USER_TYPE_BYTE:
				UINT8 byte_out;
				user_defined_out.GetByte(&byte_out);
				if (byte_out != user_defined_byte)
					sprintf(err_msg, "FAIL user defined %d byte verify", ii + 1);
				break;
			case OBEX_USER_TYPE_INT:
				UINT32 four_byte_out;
				user_defined_out.GetFourByte(&four_byte_out);
				if (four_byte_out != user_defined_int)
					sprintf(err_msg, "FAIL user defined %d four byte verify", ii + 1);
				break;
			default:
				sprintf(err_msg, "FAIL user_defined %d type out unknown", user_type_out);
				break;
			}

			// free malloced arrays
			if (p_val_string_out != NULL)
			{
				free (p_val_string_out);
				p_val_string_out = NULL;
			}
			if (p_val_array_out != NULL)
			{
				free (p_val_array_out);
				p_val_array_out = NULL;
			}
		}

		// free malloced arrays
		if (p_val_string_out != NULL)
		{
			free (p_val_string_out);
			p_val_string_out = NULL;
		}
		if (p_val_array_out != NULL)
		{
			free (p_val_array_out);
			p_val_array_out = NULL;
		}

		if ((failed) || (strlen(err_msg) != 0))
			return (err_msg);
	}

	////////////////////////////////////////////////////////////////////////////////
	// if we get this far all the fields verified
	// now delete them all and verify that you cannot 'get' them anymore

	// count header
	hdrs.DeleteCount();
	if (hdrs.GetCount(&val_count_out))
	{
		return ("FAIL count delete");
	}

	// name header
	{
		UINT32 length = 0;
		hdrs.DeleteName();
		if (hdrs.GetNameLength(&length))
			return ("FAIL name delete");
	}

	// type header
	{
		UINT32 length = 0;
		hdrs.DeleteType();
		if (hdrs.GetTypeLength(&length))
			return ("FAIL type delete");
	}

	// length header
	hdrs.DeleteLength();
	if (hdrs.GetLength(&val_length_out))
	{
		return ("FAIL length delete");
	}


	// time header
	hdrs.DeleteTime();
	{
		char time_str[25] = "";
		if (hdrs.GetTime(time_str))
			return ("FAIL time delete");
	}

	// description header
	{
		UINT32 length = 0;
		hdrs.DeleteDescription();
		if (hdrs.GetDescriptionLength(&length))
			return ("FAIL description delete");
	}


	// target header
	{
		int ii;
		UINT32 length_out = 0;

		for (ii = 0; ii < OBEX_MAX_TARGET; ii++)
		{
			if (!hdrs.DeleteTarget(0))	// forces first one out and contraction of list
				return ("FAIL target delete first");
			if (hdrs.GetTargetLength(&length_out, OBEX_MAX_TARGET - 1 - ii)) // this one should be gone
				return ("FAIL target delete non-existent");
		}
	}

	// http header
	{
		int ii;
		UINT32 length_out = 0;

		for (ii = 0; ii < OBEX_MAX_HTTP; ii++)
		{
			if (!hdrs.DeleteHttp(0))	// forces first one out and contraction of list
				return ("FAIL http delete first");
			if (hdrs.GetHttpLength(&length_out, OBEX_MAX_HTTP - 1 - ii)) // this one should be gone
				return ("FAIL http delete non-existent");
		}
	}
	
	// body header
	{
		UINT32 length = 0;

		hdrs.DeleteBody();
		if (hdrs.GetBodyLength(&length))
			return ("FAIL body delete");
	}
	
	// who header
	{
		UINT32 length = 0;
		hdrs.DeleteWho();
		if (hdrs.GetWhoLength(&length))
			return ("FAIL who delete");
	}

	// app param header
	{
		int ii;
		UINT8 length_out = 0;

		for (ii = 0; ii < OBEX_MAX_APP_PARAM; ii++)
		{
			UINT32 current_cnt = hdrs.GetAppParamCnt();
			if (current_cnt == 0)
				return ("FAIL app param delete - no entries");
			if (!hdrs.DeleteAppParam(current_cnt - 1))	// delete last existing entry in the list
				return ("FAIL app param delete last");
			if (hdrs.GetAppParamLength(&length_out, current_cnt - 1)) // this one should be gone
				return ("FAIL app param delete non-existent");
		}
	}

	// auth challenge header
	{
		int ii;
		UINT8 length_out = 0;

		for (ii = 0; ii < OBEX_MAX_AUTH_CHALLENGE; ii++)
		{
			if (!hdrs.DeleteAuthChallenge(0))	// forces first one out and contraction of list
				return ("FAIL auth challenge delete first");
			if (hdrs.GetAuthChallengeLength(&length_out, OBEX_MAX_AUTH_CHALLENGE - 1 - ii)) // this one should be gone
				return ("FAIL auth challenge delete non-existent");
		}
	}

	// auth response header
	{
		int ii;
		UINT8 length_out = 0;

		for (ii = 0; ii < OBEX_MAX_AUTH_RESPONSE; ii++)
		{
			UINT32 current_cnt = hdrs.GetAuthResponseCnt();
			if (current_cnt == 0)
				return ("FAIL auth response delete - no entries");
			if (!hdrs.DeleteAuthResponse(current_cnt - 1))	// delete last existing entry in the list
				return ("FAIL auth response delete last");
			if (hdrs.GetAuthResponseLength(&length_out, current_cnt - 1)) // this one should be gone
				return ("FAIL auth response delete non-existent");
		}
	}


	// object class header
	{
		UINT32 length = 0;
		hdrs.DeleteObjectClass();
		if (hdrs.GetObjectClassLength(&length))
			return ("FAIL object class delete");
	}


	// user defined header
	{
		int ii;
		UINT16 length_out = 0;

		for (ii = 0; ii < OBEX_MAX_USER_HDR; ii++)
		{
			if (!hdrs.DeleteUserDefined(0))	// forces first one out and contraction of list
				return ("FAIL user defined delete first");
			if (hdrs.GetUserDefinedLength(&length_out, OBEX_MAX_USER_HDR - 1 - ii)) // this one should be gone
				return ("FAIL user defined delete non-existent");
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// all headers verified and really deleted
	return ("Verify values OK for all headers");
}
