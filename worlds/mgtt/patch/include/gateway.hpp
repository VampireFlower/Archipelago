#include <types.h>
#include <os.h>

#pragma once

#define MAILBOX_SLOTS 8

#define MAX_SEND 128u

struct Frame {
    
    uint address; // mailbox address
    uint length;
    uint total_length;
    
    enum Type : uint {
        REQUEST,
        RESPONSE,
        EVENT,
        RESET
    } type;

    byte data[];
};


struct Mailbox {

    static Mailbox mailboxes[MAILBOX_SLOTS];
    

    static Mailbox* New();
    void Free();

    Mailbox* Fetch(const char* resource, void* dst = nullptr);
    Mailbox* FetchStream(const char* resource);
    void Send(void* data, uint len, Frame::Type, bool copy);

    Frame* GetNextFrame();
    void WaitForDownload();
    bool isDownloadComplete();

    static void Deliver(Frame*&);
    
    void Accept(Frame*&);

    bool allocated;

    enum SendState : byte {
        IDLE,
        WAITING_TO_SEND,
        SENDING,
        SENT
    } send_state;
    
    enum RecvState : byte {
        IDLE_,
        WAITING_FOR_RESPONSE,
        DOWNLOADING,
        DOWNLOADED
    } recv_state;
    
    uint address; // mailbox id, not memory address
    
    void* outgoing;
    uint total_outgoing;
    uint payload_sent;
    bool owns_outgoing;
    Frame::Type message_kind;

    
    bool owns_incoming;
    void* incoming;
    uint received;
    uint total_incoming;

    // when set, does not write frames to the output
    bool stream;
    union {
        OSThreadQueue on_download;
        OSThreadQueue on_frame;
    };
    Frame* handoff;
    
};

// you can't define arrays of a class inside the class itself
inline Mailbox Mailbox::mailboxes[MAILBOX_SLOTS];

inline OSThreadQueue handoff_complete;

void Gateway(void*);