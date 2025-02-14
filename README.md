# File Encryption/Decryption System

File Encryption/Decryption Operation Flow

```mermaid
%% File Encryption/Decryption System Flowchart
graph TD
    A([🏁 Start]) --> B{👤 User Action}
    B -->|📤 Upload File| C[📂 Open File]
    B -->|📥 Download File| C
    C --> D{🔄 Choose Operation}
    D -->|🔐 Encrypt| E[📜 Read File Content]
    D -->|🔓 Decrypt| E
    E --> F[📤 Submit to Process Queue]
    F --> |⚙️ Process Management|G1  

    subgraph ""  
        direction TB
        G1[📥 Consume from Queue] --> G2{⚡ Available Worker?}
        G2 -->|✅ Yes| G3[🚀 Spin Up Child Process]
        G2 -->|❌ No| G1
        G3 --> G4[🔄 Perform Encryption/Decryption]
        G4 --> G5[✅ Signal Completion]
    end

    G5 --> H{✔️ Success?}
    H -->|✅ Yes| I[💾 Write Processed File]
    H -->|❌ No| J[⚠️ Error Handling]
    I --> K([🏁 End])
    J --> K
    K --> L{🔄 Another Operation?}
    L -->|✅ Yes| B
    L -->|❌ No| M([🚪 Exit])
    
    style A fill:#4CAF50,stroke:#388E3C,stroke-width:2px
    style B fill:#1976D2,stroke:#0D47A1,stroke-width:2px
    style C fill:#4CAF50,stroke:#E65100,stroke-width:2px
    style D fill:#9C27B0,stroke:#7B1FA2,stroke-width:2px
    style F fill:#4CAF50,stroke:#FBC02D,stroke-width:2px
    style H fill:#00ACC1,stroke:#00796B,stroke-width:2px,font-weight:bold
    style I fill:#8BC34A,stroke:#558B2F,stroke-width:2px
    style J fill:#F44336,stroke:#B71C1C,stroke-width:2px
    style M fill:#9E9E9E,stroke:#424242,stroke-width:2px,font-weight:bold
