- Client \
-- 0 > Name Length <= 9 \
-- Operators \

- Channels \
-- Begin with '#' or '&' \
-- Does not contain ' ' or ^G or ','

nc 127.0.0.1 6667
PASS mdp
NICK thomi
USER ti 0 *:leti
JOIN #te
PRIVMSG #te ::message de test
