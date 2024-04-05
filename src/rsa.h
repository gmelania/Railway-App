// #include "lib.h"

class RSA {
  private:
    set<int> prime;
    int public_key;
    int private_key;
    int n;

    void primefiller();
    int pickrandomprime(mt19937 &engine);
    void setkeys(mt19937 &engine);
    long long int encrypt(double message);

  public:
    RSA();
    ~RSA() = default;

    vector<int> encoder(string message);
};

RSA::RSA() {
    primefiller();
    mt19937 engine(42);
    setkeys(engine);
}

void RSA::primefiller() {
    vector<bool> sieve(250, true);
    sieve[0] = false;
    sieve[1] = false;

    for (int i = 2; i < 250; i++) {
        for (int j = i * 2; j < 250; j += i) {
            sieve[j] = false;
        }
    }

    for (int i = 0; i < sieve.size(); i++) {
        if (sieve[i])
            prime.insert(i);
    }
}

int RSA::pickrandomprime(mt19937 &engine) {
    uniform_int_distribution<int> dist(0, prime.size() - 1);
    int k = dist(engine);
    auto it = prime.begin();
    advance(it, k);
    int ret = *it;
    prime.erase(it);
    return ret;
}

void RSA::setkeys(mt19937 &engine) {
    int prime1 = pickrandomprime(engine);
    int prime2 = pickrandomprime(engine);

    n = prime1 * prime2;
    int fi = (prime1 - 1) * (prime2 - 1);
    int e = 2;

    while (1) {
        if (std::__gcd(e, fi) == 1)
            break;
        e++;
    }

    public_key = e;
    int d = 2;

    while (1) {
        if ((d * e) % fi == 1)
            break;
        d++;
    }

    private_key = d;
}

long long int RSA::encrypt(double message) {
    int e = public_key;
    long long int encrypted_text = 1;

    while (e--) {
        encrypted_text *= message;
        encrypted_text %= n;
    }

    return encrypted_text;
}

vector<int>
RSA::encoder(string message) { // vect int = encoder de string message
    vector<int> form;

    for (auto &letter : message)
        form.push_back(encrypt(static_cast<int>(letter)));

    return form;
}