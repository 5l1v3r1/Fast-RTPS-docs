


#ifndef _UNITTEST_SECURITY_CRYPTOGRAPHY_CRYPTOGRAPHYPLUGINTESTS_HPP_
#define _UNITTEST_SECURITY_CRYPTOGRAPHY_CRYPTOGRAPHYPLUGINTESTS_HPP_

#include "../../../../src/cpp/security/cryptography/AESGCMGMAC.h"

#include <gtest/gtest.h>

using namespace eprosima::fastrtps::rtps;
using namespace ::security;

class CryptographyPluginTest : public ::testing::Test{

    protected:
        virtual void SetUp(){
            PropertyPolicy m_propertypolicy;

            CryptoPlugin = new AESGCMGMAC(m_propertypolicy);
            
        }
        virtual void TearDown(){
            delete CryptoPlugin;

        }

    public:
        CryptographyPluginTest():CryptoPlugin(nullptr){};

        static IdentityHandle* create_placeholder_identityhandle();
        static PermissionsHandle* create_placeholder_permissionshandle();
        static PropertySeq* create_placeholder_propertyseq();
    
        static bool check_localparticipanthandle(PropertySeq &properties, ParticipantCryptoHandle *local);

        AESGCMGMAC* CryptoPlugin;

};



TEST_F(CryptographyPluginTest, factory_createLocalParticipantHandle)
{

    IdentityHandle* i_handle = CryptographyPluginTest::create_placeholder_identityhandle();
    PermissionsHandle* perm_handle = CryptographyPluginTest::create_placeholder_permissionshandle();
    PropertySeq* prop_handle = CryptographyPluginTest::create_placeholder_propertyseq();
    SecurityException exception;

    ParticipantCryptoHandle *target = CryptoPlugin->m_cryptokeyfactory->register_local_participant(*i_handle,*perm_handle,*prop_handle,exception);

    ASSERT_TRUE(target != nullptr);

    //Dissect results to check correct creation
    ASSERT_TRUE(CryptographyPluginTest::check_localparticipanthandle(*prop_handle, target) );
    
}
TEST_F(CryptographyPluginTest, factory_RegisterRemoteParticipant)
{
    
    IdentityHandle* i_handle = CryptographyPluginTest::create_placeholder_identityhandle();
    PermissionsHandle* perm_handle = CryptographyPluginTest::create_placeholder_permissionshandle();
    PropertySeq* prop_handle = CryptographyPluginTest::create_placeholder_propertyseq();
    SecurityException exception;

    ParticipantCryptoHandle *local = CryptoPlugin->m_cryptokeyfactory->register_local_participant(*i_handle,*perm_handle,*prop_handle,exception);
    ParticipantCryptoHandle *remote = CryptoPlugin->m_cryptokeyfactory->register_local_participant(*i_handle,*perm_handle,*prop_handle,exception);

    ASSERT_TRUE( (local != nullptr)&(remote != nullptr));


    //Dissect results to check correct creation
    ASSERT_TRUE(CryptographyPluginTest::check_localparticipanthandle(*prop_handle, local) );
    ASSERT_TRUE(CryptographyPluginTest::check_localparticipanthandle(*prop_handle, remote) );
    AESGCMGMAC_ParticipantCryptoHandle& local_participant = AESGCMGMAC_ParticipantCryptoHandle::narrow(*local);
    AESGCMGMAC_ParticipantCryptoHandle& remote_participant = AESGCMGMAC_ParticipantCryptoHandle::narrow(*remote);

    //Check handles are different (safety)
    ASSERT_TRUE(local_participant->ParticipantKeyMaterial->master_salt != remote_participant->ParticipantKeyMaterial->master_salt);
    ASSERT_TRUE(local_participant->ParticipantKeyMaterial->sender_key_id != remote_participant->ParticipantKeyMaterial->sender_key_id);
    ASSERT_TRUE(local_participant->ParticipantKeyMaterial->master_sender_key != remote_participant->ParticipantKeyMaterial->master_sender_key);




}

#endif