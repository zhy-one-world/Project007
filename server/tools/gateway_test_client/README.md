# Gateway Test Client

`gateway_test_client` is a standalone load-test executable for the Gateway external
TCP endpoint. It uses the same `tcp_client`, packet format, and generated
Protobuf messages as the server.

## Build

Configure the server solution as usual, then build the target:

```powershell
cmake -S E:/Project/Project007/server -B E:/Project/Project007/server/build-gateway `
  -G "Visual Studio 18 2026" -A x64
cmake --build E:/Project/Project007/server/build-gateway `
  --config Release --target gateway_test_client
```

The `SeverBase` libraries and third-party dependencies must be available in
the same locations used by the existing Gateway target.

## Login probe

After config_center → gateway → ws are up:

```powershell
.\gateway_test_client.exe --mode login `
  --config-center-host 127.0.0.1 --config-center-port 19000 `
  --server-id 10200 --account test_account --password 123456 --duration 30
```

Flow: `GET /v1/registry` (gateway external list only) → filter `game_id=10200` →
login TCP. Multi-gateway via `--gateway-index`.

Expect: RDE handshake, then `e_msgindex_c2s_client_login` forwarded gateway→ws.
Full auth still needs DP; without DP the probe verifies the forward path.

## Run

The default target is `127.0.0.1:2200` (gateway **external** client port):

```powershell
.\gateway_test_client.exe --connections 100 --duration 60 --interval 1000
```

Available options:

```text
--host <host>              Gateway host (default: 127.0.0.1)
--port <port>              Gateway port (default: 2200)
--connections <count>      concurrent connections (default: 100)
--duration <seconds>       test duration; 0 runs until Ctrl+C (default: 60)
--interval <milliseconds>  ping interval (default: 1000)
--threads <count>          client scheduler threads (default: 4)
--mode <load|login>        load stress or single login probe (default: load)
--account <name>           login account (login mode)
--password <pwd>           login password (login mode)
--server-id <id>           login server_id (default: 10200)
```

Each connection waits for the server's `10100` RDE handshake packet, then
sends `10002` Ping packets and validates `10003` responses. The current
repository does not contain the RDE client algorithm; the existing Gateway
security layer forwards these packets without encryption, so this tool does
not implement or pretend to implement RDE encryption.

Every connection has its own client object and is assigned to a scheduler
worker in round-robin order. Connection, retry, Ping, and response handling
are dispatched through that connection's worker.
