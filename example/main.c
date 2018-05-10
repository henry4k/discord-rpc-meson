#include <stdbool.h>
#include <stdio.h> // printf
#include <stdint.h> // int64_t
#include <string.h> // memset
#include <time.h> // time
#include <signal.h> // signal
#include <unistd.h> // sleep
#include <discord_rpc.h>

static const char* APPLICATION_ID = "378591061519630338";
static int64_t StartTime;
static bool ContinueLoop = true;

static void OnReady()
{
    printf("ready\n");
}

static void OnDisconnected(int errorCode, const char* message)
{
    printf("disconnected: %s (%d)\n", message, errorCode);
}

static void OnError(int errorCode, const char* message)
{
    printf("error: %s (%d)\n", message, errorCode);
}

static void OnJoin(const char* secret)
{
    printf("join: %s\n", secret);
}

static void OnSpectate(const char* secret)
{
    printf("spectate: %s\n", secret);
}

static void OnJoinRequest(const DiscordUser* user)
{
    printf("join request: username=%s\n"
           "              discriminator=%s\n"
           "              userId=%s\n",
           user->username,
           user->discriminator,
           user->userId);
    Discord_Respond(user->userId, DISCORD_REPLY_YES);
}

static void OnStopSignal(int sig)
{
    ContinueLoop = false;
    printf("stop\n");
}

static void Initialize()
{
    StartTime = time(NULL);

    signal(SIGTERM, OnStopSignal);
    signal(SIGINT,  OnStopSignal);

    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready        = OnReady;
    handlers.disconnected = OnDisconnected;
    handlers.errored      = OnError;
    handlers.joinGame     = OnJoin;
    handlers.spectateGame = OnSpectate;
    handlers.joinRequest  = OnJoinRequest;
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

static void UpdatePresence()
{
    DiscordRichPresence presence;
    memset(&presence, 0, sizeof(presence));
    presence.state = "state goes here";
    presence.details = "details go here";
    presence.startTimestamp = StartTime;
    presence.endTimestamp = StartTime + 5 * 60;
    presence.largeImageKey = "test";
    presence.largeImageText = "tooltip goes here";
    //presence.smallImageKey = "ptb-small";
    //presence.partyId = "party1234";
    presence.partySize = 1;
    presence.partyMax = 6;
    //presence.matchSecret = "xyzzy";
    //presence.joinSecret = "join";
    //presence.spectateSecret = "look";
    //presence.instance = 0;
    Discord_UpdatePresence(&presence);
}

int main()
{
    Initialize();

    UpdatePresence();

    while(ContinueLoop)
    {
        sleep(1);

#ifdef DISCORD_DISABLE_IO_THREAD
        Discord_UpdateConnection();
#endif
        Discord_RunCallbacks();
    }

    Discord_Shutdown();
}
