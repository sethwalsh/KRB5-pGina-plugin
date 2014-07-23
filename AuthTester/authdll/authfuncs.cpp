#include "authfuncs.h"

extern "C"{
	__declspec(dllexport) int auth_user(char* u, char* p, char* d, char* t)
	{
		/*
			Creates a new SEC_WINNT_AUTH_IDENTITY structure using the given user name, password, and domain.  These fields are supplied by pGina, and the configuration
			tool for the krb5 plugin.
		*/
		SEC_WINNT_AUTH_IDENTITY auth;
		auth.Domain = reinterpret_cast<unsigned char*>( d );
		auth.DomainLength = strlen( d );
		auth.User = reinterpret_cast<unsigned char*>( u );
		auth.UserLength = strlen( u );
		auth.Password = reinterpret_cast<unsigned char*>( p );
		auth.PasswordLength = strlen( p );
		auth.Flags = SEC_WINNT_AUTH_IDENTITY_ANSI;

		char clientOutBufferData[8192];
		char serverOutBufferData[8192];

		SecBuffer     clientOutBuffer;
		SecBufferDesc clientOutBufferDesc;

		SecBuffer     serverOutBuffer[8192];
		SecBufferDesc serverOutBufferDesc[8192];

		///////////////////////////////////////////
		// Get the client and server credentials //
		///////////////////////////////////////////

		CredHandle clientCredentials;
		CredHandle serverCredentials;

		SECURITY_STATUS status;

		/*
			Acquires a HANDLE to the credentials for the given user
		*/
		status = ::AcquireCredentialsHandle( NULL,
											 "Kerberos",
											 SECPKG_CRED_OUTBOUND,
											 NULL,
											 &auth,
											 NULL,
											 NULL,
											 &clientCredentials,
											 NULL );

		if(status != SEC_E_OK)
			return status;

		//////////////////////////////////////
		// Initialize the security contexts //
		//////////////////////////////////////

		CtxtHandle clientContext = {};
		unsigned long clientContextAttr = 0;

		CtxtHandle serverContext = {};
		unsigned long serverContextAttr = 0;

		/////////////////////////////
		// Clear the client buffer //
		/////////////////////////////

		clientOutBuffer.BufferType = SECBUFFER_TOKEN;
		clientOutBuffer.cbBuffer   = sizeof clientOutBufferData;
		clientOutBuffer.pvBuffer   = clientOutBufferData;

		clientOutBufferDesc.cBuffers  = 1;
		clientOutBufferDesc.pBuffers  = &clientOutBuffer;
		clientOutBufferDesc.ulVersion = SECBUFFER_VERSION;

		///////////////////////////////////
		// Initialize the client context //
		///////////////////////////////////

		status = InitializeSecurityContext( &clientCredentials,
											NULL,
											t,// the (service/domain) spn target that will authenticate this user "krbtgt/ad.utah.edu",
											0,
											0,
											SECURITY_NATIVE_DREP,
											NULL,
											0,
											&clientContext,
											&clientOutBufferDesc,
											&clientContextAttr,
											NULL );

		return status;
	}
}