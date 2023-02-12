int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: client_tester <host> <port> <request>" << std::endl;
        return 1;
    }

    ClientTester tester(argv[1], argv[2], argv[3]);
    tester.ConnectAndSendRequest();

    return 0;
}