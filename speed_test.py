import asyncio

from networking.tcp import Server, Session, MessageHandler

class ServerTask(MessageHandler):
    def __init__(self, port: int):
        super().__init__()
        self._server = Server(port = port, message_handler = self)
        self._sessions = set()

    async def run(self):
        await self._server.run()

    async def on_connect(self, session: Session):
        print("Connection from: %s" % session.remote_endpoint)
        self._sessions.add(session)

    async def on_disconnect(self, session: Session):
        print("Disconnected from: %s" % session.remote_endpoint)
        self._sessions.remove(session)

    async def on_received(self, session: Session, buffer: bytes, timestamp: float):
        print(f"Received {len(buffer)} bytes from {session.remote_endpoint}")

if __name__ == "__main__":
    loop = asyncio.new_event_loop()
    server = ServerTask(port = 8080)
    tasks = [ ]
    tasks.append(loop.create_task(server.run()))
    loop.run_until_complete(asyncio.gather(*tasks))