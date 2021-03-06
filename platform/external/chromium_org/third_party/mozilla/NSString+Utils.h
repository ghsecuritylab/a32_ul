/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Chimera code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2002
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Simon Fraser <sfraser@netscape.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#import <Foundation/Foundation.h>

typedef enum
{
  kTruncateAtStart,
  kTruncateAtMiddle,
  kTruncateAtEnd
} ETruncationType;


@interface NSString (ChimeraStringUtils)

+ (id)ellipsisString;
+ (NSString*)stringWithUUID;

- (BOOL)isEqualToStringIgnoringCase:(NSString*)inString;
- (BOOL)hasCaseInsensitivePrefix:(NSString*)inString;

- (BOOL)isLooselyValidatedURI;

- (BOOL)isPotentiallyDangerousURI;

- (BOOL)isValidURI;

- (NSString *)stringByRemovingCharactersInSet:(NSCharacterSet*)characterSet;
- (NSString *)stringByReplacingCharactersInSet:(NSCharacterSet*)characterSet withString:(NSString*)string;
- (NSString *)stringByTruncatingTo:(unsigned int)maxCharacters at:(ETruncationType)truncationType;
- (NSString *)stringByTruncatingToWidth:(float)inWidth at:(ETruncationType)truncationType withAttributes:(NSDictionary *)attributes;
- (NSString *)stringByTrimmingWhitespace;
- (NSString *)stringByRemovingAmpEscapes;
- (NSString *)stringByAddingAmpEscapes;

@end

@interface NSMutableString (ChimeraMutableStringUtils)

- (void)truncateTo:(unsigned)maxCharacters at:(ETruncationType)truncationType;
- (void)truncateToWidth:(float)maxWidth at:(ETruncationType)truncationType withAttributes:(NSDictionary *)attributes;

@end

@interface NSString (ChimeraFilePathStringUtils)

- (NSString*)volumeNamePathComponent;
- (NSString*)displayNameOfLastPathComponent;

@end

@interface NSString (CaminoURLStringUtils)

- (BOOL)isBlankURL;
- (NSString *)unescapedURI;

@end
